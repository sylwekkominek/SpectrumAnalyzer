#include "RectanglesVisibilityState.hpp"

RectanglesVisibilityState::RectanglesVisibilityState(bool value) : value(value)
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

template<>
bool RectanglesVisibilityState::getRectanglesVisibilityState<Mode::Analyzer>(const ThemeConfig themeConfig)
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

template<>
bool RectanglesVisibilityState::getRectanglesVisibilityState<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    return true;
}

RectanglesVisibilityState::RectanglesVisibilityState(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getRectanglesVisibilityState<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getRectanglesVisibilityState<Mode::Visualizer>(themeConfig);
        break;
    }
}

