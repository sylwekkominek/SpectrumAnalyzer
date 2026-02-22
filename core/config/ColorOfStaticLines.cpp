#include "ColorOfStaticLines.hpp"

ColorOfStaticLines::ColorOfStaticLines(const Color &value) : value(value)
{
}

ColorOfStaticLines::ColorOfStaticLines(const ThemeConfig themeConfig) : value(getColorOfStaticLines(themeConfig))
{
}

std::string ColorOfStaticLines::getInfo()
{
    return std::string(
        R"(//Description: Allows you to customize the color (RGB) and transparency of static lines that remain constant on the display.
//Default value: RGBA color (Red, Green, Blue, Transparency)
)");
}

std::ostream& operator<<(std::ostream& os, const ColorOfStaticLines &colorOfStaticLines)
{
    os<<"colorOfStaticLines: ";
    for(auto & component: colorOfStaticLines.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
}

Color ColorOfStaticLines::getColorOfStaticLines(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {0.2, 0.2, 0.2, 1.00};
    }
}
