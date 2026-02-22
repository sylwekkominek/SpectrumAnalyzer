#include "DynamicMaxHoldSecondarySpeedOfFalling.hpp"

DynamicMaxHoldSecondarySpeedOfFalling::DynamicMaxHoldSecondarySpeedOfFalling(float value) : value(value)
{
}

DynamicMaxHoldSecondarySpeedOfFalling::DynamicMaxHoldSecondarySpeedOfFalling(const ThemeConfig themeConfig) : value(getDynamicMaxHoldSecondarySpeedOfFalling(themeConfig))
{
}

std::string DynamicMaxHoldSecondarySpeedOfFalling::getInfo()
{
    return std::string(
        R"(//Description: Sets how fast the secondary max hold markers decrease in value, affecting the speed at which the peak indicators move downward.
//Default value: 1000
)");
}

float DynamicMaxHoldSecondarySpeedOfFalling::getDynamicMaxHoldSecondarySpeedOfFalling(const ThemeConfig themeConfig)
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

        default:
            return 1000;
    }
}
