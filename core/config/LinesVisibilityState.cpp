#include "LinesVisibilityState.hpp"


LinesVisibilityState::LinesVisibilityState(bool value) : value(value)
{
}

std::string LinesVisibilityState::getInfo()
{
    return std::string(
        R"(//Description: Enables the visibility of lines on the display.
//Default value: false
)");
}

std::ostream& operator<<(std::ostream& os, const LinesVisibilityState &linesVisibilityState)
{
    os <<"linesVisibilityState: "<<linesVisibilityState.value<<std::endl;
    return os;
}

template<>
bool LinesVisibilityState::getLinesVisibilityState<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme2:
            return true;
        case ThemeConfig::Theme3:
            return true;
        case ThemeConfig::Theme4:
            return true;
        case ThemeConfig::Theme5:
            return true;
        case ThemeConfig::Theme6:
            return true;
        case ThemeConfig::Theme7:
            return true;
        case ThemeConfig::Theme9:
            return true;
        case ThemeConfig::Theme10:
            return true;
        default:
            return false;
    }
}

template<>
bool LinesVisibilityState::getLinesVisibilityState<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    return false;
}

LinesVisibilityState::LinesVisibilityState(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getLinesVisibilityState<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getLinesVisibilityState<Mode::Visualizer>(themeConfig);
        break;
    }
}
