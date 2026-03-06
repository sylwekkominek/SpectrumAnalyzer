#include "DynamicMaxHoldSpeedOfFalling.hpp"

DynamicMaxHoldSpeedOfFalling::DynamicMaxHoldSpeedOfFalling(float value) : value(value)
{
}

std::string DynamicMaxHoldSpeedOfFalling::getInfo()
{
    return std::string(
        R"(//Description: Sets how fast the max hold markers decrease in value, affecting the speed at which the peak indicators move downward.
//Default value: 900
)");
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldSpeedOfFalling &dynamicMaxHoldSpeedOfFalling)
{
    os <<"dynamicMaxHoldSpeedOfFalling: "<<dynamicMaxHoldSpeedOfFalling.value<<std::endl;
    return os;
}

template<>
float DynamicMaxHoldSpeedOfFalling::getDynamicMaxHoldSpeedOfFalling<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 900;
    }
}

template<>
float DynamicMaxHoldSpeedOfFalling::getDynamicMaxHoldSpeedOfFalling<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 900;
    }
}

DynamicMaxHoldSpeedOfFalling::DynamicMaxHoldSpeedOfFalling(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getDynamicMaxHoldSpeedOfFalling<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getDynamicMaxHoldSpeedOfFalling<Mode::Visualizer>(themeConfig);
        break;
    }
}
