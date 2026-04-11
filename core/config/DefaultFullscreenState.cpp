#include "DefaultFullscreenState.hpp"

DefaultFullscreenState::DefaultFullscreenState(bool value) : value(value)
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

template<>
bool DefaultFullscreenState::getDefaultFullscreenState<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return false;
    }
}

template<>
bool DefaultFullscreenState::getDefaultFullscreenState<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return false;
    }
}

template<>
bool DefaultFullscreenState::getDefaultFullscreenState<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return false;
    }
}

DefaultFullscreenState::DefaultFullscreenState(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getDefaultFullscreenState<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getDefaultFullscreenState<Mode::Visualizer>(themeConfig);
        break;
    case Mode::StereoRmsMeter:
        value = getDefaultFullscreenState<Mode::StereoRmsMeter>(themeConfig);
        break;
    }
}
