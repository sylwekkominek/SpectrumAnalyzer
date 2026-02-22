#include "DynamicMaxHoldSpeedOfFalling.hpp"

DynamicMaxHoldSpeedOfFalling::DynamicMaxHoldSpeedOfFalling(float value) : value(value)
{
}

DynamicMaxHoldSpeedOfFalling::DynamicMaxHoldSpeedOfFalling(const ThemeConfig themeConfig) : value(getDynamicMaxHoldSpeedOfFalling(themeConfig))
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

float DynamicMaxHoldSpeedOfFalling::getDynamicMaxHoldSpeedOfFalling(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 900;
    }
}
