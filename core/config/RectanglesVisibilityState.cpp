#include "RectanglesVisibilityState.hpp"

RectanglesVisibilityState::RectanglesVisibilityState(bool value) : value(value)
{
}

RectanglesVisibilityState::RectanglesVisibilityState(const ThemeConfig themeConfig) : value(getRectanglesVisibilityState(themeConfig))
{
}

std::string RectanglesVisibilityState::getInfo()
{
    return std::string(
        R"(//Description: Enables the visibility of rectangles on the display.
//Default value: true
)");
}

std::ostream& operator<<(std::ostream& os, const RectanglesVisibilityState &rectanglesVisibilityState)
{
    os <<"rectanglesVisibilityState: "<<rectanglesVisibilityState.value<<std::endl;
    return os;
}

bool RectanglesVisibilityState::getRectanglesVisibilityState(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme6:
            return false;
        case ThemeConfig::Theme7:
            return false;
        default:
            return true;
    }
}
