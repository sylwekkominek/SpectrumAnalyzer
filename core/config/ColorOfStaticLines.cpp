#include "ColorOfStaticLines.hpp"

ColorOfStaticLines::ColorOfStaticLines(const Color &value) : value(value)
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

template<>
Color ColorOfStaticLines::getColorOfStaticLines<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {0.2, 0.2, 0.2, 1.00};
    }
}

template<>
Color ColorOfStaticLines::getColorOfStaticLines<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {0.2, 0.2, 0.2, 1.00};
    }
}

ColorOfStaticLines::ColorOfStaticLines(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getColorOfStaticLines<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getColorOfStaticLines<Mode::Visualizer>(themeConfig);
        break;
    }
}
