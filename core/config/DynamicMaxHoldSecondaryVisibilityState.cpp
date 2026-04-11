#include "DynamicMaxHoldSecondaryVisibilityState.hpp"


DynamicMaxHoldSecondaryVisibilityState::DynamicMaxHoldSecondaryVisibilityState(bool value) : value(value)
{
}

std::string DynamicMaxHoldSecondaryVisibilityState::getInfo()
{
    return std::string(
        R"(//Description: Toggles the visibility of secondary on-screen elements that display the peak (max hold) values over time. When enabled, secondary markers show the highest recent signal levels.
//Default value: True
)");
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldSecondaryVisibilityState &dynamicMaxHoldSecondaryVisibilityState)
{
    os <<"dynamicMaxHoldSecondaryVisibilityState: "<<dynamicMaxHoldSecondaryVisibilityState.value<<std::endl;
    return os;
}

template<>
bool DynamicMaxHoldSecondaryVisibilityState::getDynamicMaxHoldSecondaryVisibilityState<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return true;
    }
}

template<>
bool DynamicMaxHoldSecondaryVisibilityState::getDynamicMaxHoldSecondaryVisibilityState<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return true;
    }
}

template<>
bool DynamicMaxHoldSecondaryVisibilityState::getDynamicMaxHoldSecondaryVisibilityState<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return true;
    }
}

DynamicMaxHoldSecondaryVisibilityState::DynamicMaxHoldSecondaryVisibilityState(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getDynamicMaxHoldSecondaryVisibilityState<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getDynamicMaxHoldSecondaryVisibilityState<Mode::Visualizer>(themeConfig);
        break;
    case Mode::StereoRmsMeter:
        value = getDynamicMaxHoldSecondaryVisibilityState<Mode::StereoRmsMeter>(themeConfig);
        break;
    }
}
