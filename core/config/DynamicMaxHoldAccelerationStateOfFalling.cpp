#include "DynamicMaxHoldAccelerationStateOfFalling.hpp"

DynamicMaxHoldAccelerationStateOfFalling::DynamicMaxHoldAccelerationStateOfFalling(float value) : value(value)
{
}

std::string DynamicMaxHoldAccelerationStateOfFalling::getInfo()
{
    return std::string(
        R"(//Description: Determines whether the max hold elements fall with acceleration or at a constant speed. This gives the user control over the visual behavior and final effect of the peak indicators.
//Default value: True
)");
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldAccelerationStateOfFalling & dynamicMaxHoldAccelerationStateOfFalling)
{
    os <<"dynamicMaxHoldAccelerationStateOfFalling: "<<dynamicMaxHoldAccelerationStateOfFalling.value<<std::endl;
    return os;
}

template<>
bool DynamicMaxHoldAccelerationStateOfFalling::getDynamicMaxHoldAccelerationStateOfFalling<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return true;
    }
}

template<>
bool DynamicMaxHoldAccelerationStateOfFalling::getDynamicMaxHoldAccelerationStateOfFalling<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return true;
    }
}

template<>
bool DynamicMaxHoldAccelerationStateOfFalling::getDynamicMaxHoldAccelerationStateOfFalling<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return true;
    }
}

DynamicMaxHoldAccelerationStateOfFalling::DynamicMaxHoldAccelerationStateOfFalling(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getDynamicMaxHoldAccelerationStateOfFalling<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getDynamicMaxHoldAccelerationStateOfFalling<Mode::Visualizer>(themeConfig);
        break;
    case Mode::StereoRmsMeter:
        value = getDynamicMaxHoldAccelerationStateOfFalling<Mode::StereoRmsMeter>(themeConfig);
        break;
    }
}
