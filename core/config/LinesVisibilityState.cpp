#include "LinesVisibilityState.hpp"


LinesVisibilityState::LinesVisibilityState(bool value) : value(value)
{
}

LinesVisibilityState::LinesVisibilityState(const ThemeConfig themeConfig) : value(getLinesVisibilityState(themeConfig))
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

bool LinesVisibilityState::getLinesVisibilityState(const ThemeConfig themeConfig)
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
    default:
        return false;
    }
}
