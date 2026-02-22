#include "ColorOfDynamicMaxHoldSecondaryLine.hpp"


ColorOfDynamicMaxHoldSecondaryLine::ColorOfDynamicMaxHoldSecondaryLine(const Color &value) : value(value)
{
}

ColorOfDynamicMaxHoldSecondaryLine::ColorOfDynamicMaxHoldSecondaryLine(const ThemeConfig themeConfig) : value(getColorOfDynamicMaxHoldSecondaryLine(themeConfig))
{
}

std::string ColorOfDynamicMaxHoldSecondaryLine::getInfo()
{
    return std::string(
        R"(//Description: Allows you to customize the color (RGB) and transparency of secondary dynamic max hold lines that hold and display the peak (max hold) values over time.
//Default value: RGBA color (Red, Green, Blue, Transparency)
)");
}

std::ostream& operator<<(std::ostream& os, const ColorOfDynamicMaxHoldSecondaryLine &colorOfDynamicMaxHoldSecondaryLine)
{
    os<<"colorOfDynamicMaxHoldSecondaryLine: ";
    for(auto & component: colorOfDynamicMaxHoldSecondaryLine.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
}

Color ColorOfDynamicMaxHoldSecondaryLine::getColorOfDynamicMaxHoldSecondaryLine(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {0.7, 0.7, 0.7, 1.00};
        }
}
