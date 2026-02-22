#include "ColorOfStaticText.hpp"

ColorOfStaticText::ColorOfStaticText(const Color &value) : value(value)
{
}

ColorOfStaticText::ColorOfStaticText(const ThemeConfig themeConfig) : value(getColorOfStaticText(themeConfig))
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

Color ColorOfStaticText::getColorOfStaticText(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {0.50, 0.50, 0.50, 1.00};
    }
}
