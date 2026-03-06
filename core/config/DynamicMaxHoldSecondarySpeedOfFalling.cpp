#include "DynamicMaxHoldSecondarySpeedOfFalling.hpp"

DynamicMaxHoldSecondarySpeedOfFalling::DynamicMaxHoldSecondarySpeedOfFalling(float value) : value(value)
{
}

std::string DynamicMaxHoldSecondarySpeedOfFalling::getInfo()
{
    return std::string(
        R"(//Description: Sets how fast the secondary max hold markers decrease in value, affecting the speed at which the peak indicators move downward.
//Default value: 1000
)");
}

template<>
float DynamicMaxHoldSecondarySpeedOfFalling::getDynamicMaxHoldSecondarySpeedOfFalling<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme2:
            return 2000;
        case ThemeConfig::Theme3:
            return 2000;
        case ThemeConfig::Theme4:
            return 2000;
        case ThemeConfig::Theme5:
            return 2000;
        case ThemeConfig::Theme6:
            return 2000;
        case ThemeConfig::Theme9:
            return 2000;
        case ThemeConfig::Theme10:
            return 2000;

        default:
            return 1000;
    }
}

template<>
float DynamicMaxHoldSecondarySpeedOfFalling::getDynamicMaxHoldSecondarySpeedOfFalling<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    return 1000;
}

DynamicMaxHoldSecondarySpeedOfFalling::DynamicMaxHoldSecondarySpeedOfFalling(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getDynamicMaxHoldSecondarySpeedOfFalling<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getDynamicMaxHoldSecondarySpeedOfFalling<Mode::Visualizer>(themeConfig);
        break;
    }
}
