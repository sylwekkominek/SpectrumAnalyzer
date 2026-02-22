#include "ColorOfLine.hpp"

ColorOfLine::ColorOfLine(const Color &value) : value(value)
{
}

ColorOfLine::ColorOfLine(const ThemeConfig themeConfig) : value(getColorOfLine(themeConfig))
{
}

std::string ColorOfLine::getInfo()
{
    return std::string(
        R"(//Description: Allows you to customize the color (RGB) and transparency of lines that display changing dBFS values on the screen.
//Default value: RGBA color (Red, Green, Blue, Transparency)
)");
}

std::ostream& operator<<(std::ostream& os, const ColorOfLine &colorOfLine)
{
    os<<"colorOfLine: ";
    for(auto & component: colorOfLine.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
}

Color ColorOfLine::getColorOfLine(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme2:
            return {0.35, 0.55, 1.0, 1.0};
        case ThemeConfig::Theme3:
            return {0.95, 0.85, 0.3, 1.0};
        case ThemeConfig::Theme4:
            return {1.0, 0.35, 0.45, 1.0};
        case ThemeConfig::Theme5:
            return {0.35, 1.0, 0.45, 1.0};
        case ThemeConfig::Theme6:
            return {0.2, 0.4, 1.0, 1.0};
        case ThemeConfig::Theme7:
            return {0.2, 0.4, 1.0, 1.0};
        default:
            return {1,0.15,0.15,1};
    }
}
