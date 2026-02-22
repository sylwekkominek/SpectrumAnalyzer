#include "DefaultFullscreenState.hpp"

DefaultFullscreenState::DefaultFullscreenState(bool value) : value(value)
{
}

DefaultFullscreenState::DefaultFullscreenState(const ThemeConfig themeConfig) : value(getDefaultFullscreenState(themeConfig))
{
}

std::string DefaultFullscreenState::getInfo()
{
    return std::string(
        R"(//Description: Determines whether the SpectrumAnalyzer application starts in fullscreen mode by default. Users can choose this to enable fullscreen startup automatically. This is especially useful on devices like Raspberry Pi, where fullscreen improves user experience and system integration.
//Default value: False (starts in windowed mode)
)");
}

std::ostream& operator<<(std::ostream& os, const DefaultFullscreenState& defaultFullscreenState)
{
    os <<"defaultFullscreenState: "<<defaultFullscreenState.value<<std::endl;
    return os;
}

bool DefaultFullscreenState::getDefaultFullscreenState(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return false;
    }
}
