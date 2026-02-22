#include "ColorOfDynamicMaxHoldLine.hpp"

ColorOfDynamicMaxHoldLine::ColorOfDynamicMaxHoldLine(const Color &value) : value(value)
{
}

ColorOfDynamicMaxHoldLine::ColorOfDynamicMaxHoldLine(const ThemeConfig themeConfig) : value(getColorOfDynamicMaxHoldLine(themeConfig))
{
}

std::string ColorOfDynamicMaxHoldLine::getInfo()
{
    return std::string(
        R"(//Description: Allows you to customize the color (RGB) and transparency of dynamic max hold lines that hold and display the peak (max hold) values over time.
//Default value: RGBA color (Red, Green, Blue, Transparency)
)");
}

std::ostream& operator<<(std::ostream& os, const ColorOfDynamicMaxHoldLine &colorOfDynamicMaxHoldLine)
{
    os<<"colorOfDynamicMaxHoldLine: ";
    for(auto & component: colorOfDynamicMaxHoldLine.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
}

Color ColorOfDynamicMaxHoldLine::getColorOfDynamicMaxHoldLine(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {0.15,1,0.15,1};
        }
}
