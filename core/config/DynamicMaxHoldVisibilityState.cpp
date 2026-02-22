#include "DynamicMaxHoldVisibilityState.hpp"


DynamicMaxHoldVisibilityState::DynamicMaxHoldVisibilityState(bool value) : value(value)
{
}

DynamicMaxHoldVisibilityState::DynamicMaxHoldVisibilityState(const ThemeConfig themeConfig) : value(getDynamicMaxHoldVisibilityState(themeConfig))
{
}

std::string DynamicMaxHoldVisibilityState::getInfo()
{
    return std::string(
        R"(//Description: Toggles the visibility of on-screen elements that display the peak (max hold) values over time. When enabled, markers show the highest recent signal levels.
//Default value: True
)");
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldVisibilityState &dynamicMaxHoldVisibilityState)
{
    os <<"dynamicMaxHoldVisibilityState: "<<dynamicMaxHoldVisibilityState.value<<std::endl;
    return os;
}

bool DynamicMaxHoldVisibilityState::getDynamicMaxHoldVisibilityState(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme2:
            return false;
        case ThemeConfig::Theme3:
            return false;
        case ThemeConfig::Theme4:
            return false;
        case ThemeConfig::Theme5:
            return false;
        case ThemeConfig::Theme6:
            return false;
        case ThemeConfig::Theme7:
            return false;
        default:
            return true;
    }
}
