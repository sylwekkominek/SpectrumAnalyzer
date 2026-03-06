#include "ColorOfStaticText.hpp"

ColorOfStaticText::ColorOfStaticText(const Color &value) : value(value)
{
}

std::string ColorOfStaticText::getInfo()
{
    return std::string(
        R"(//Description: Allows you to customize the color (RGB) and transparency of static text that remains constant on the display.
//Default value: RGBA color (Red, Green, Blue, Transparency)
)");
}

std::ostream& operator<<(std::ostream& os, const ColorOfStaticText &colorOfStaticText)
{
    os<<"colorOfStaticText: ";
    for(auto & component: colorOfStaticText.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
}

template<>
Color ColorOfStaticText::getColorOfStaticText<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {0.50, 0.50, 0.50, 1.00};
    }
}

template<>
Color ColorOfStaticText::getColorOfStaticText<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {0.50, 0.50, 0.50, 1.00};
    }
}

ColorOfStaticText::ColorOfStaticText(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getColorOfStaticText<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getColorOfStaticText<Mode::Visualizer>(themeConfig);
        break;
    }
}

