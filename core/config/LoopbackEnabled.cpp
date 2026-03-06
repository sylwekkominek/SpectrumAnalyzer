#include "LoopbackEnabled.hpp"

LoopbackEnabled::LoopbackEnabled(bool value) : value(value)
{
}

std::string LoopbackEnabled::getInfo()
{
    return std::string(
        R"(//If this value is true, the audio will be captured from what is currently playing on the system instead of from the microphone or another input source.)");
}

std::ostream& operator<<(std::ostream& os, const LoopbackEnabled &loopbackEnabled)
{
    os <<"loopbackEnabled: "<<loopbackEnabled.value<<std::endl;
    return os;
}

template<>
bool LoopbackEnabled::getLoopbackEnabled<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme8:
            return false;
        case ThemeConfig::Theme9:
            return false;
        case ThemeConfig::Theme10:
            return false;
        default:
            return true;
    }
}

template<>
bool LoopbackEnabled::getLoopbackEnabled<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme8:
            return false;
        case ThemeConfig::Theme9:
            return false;
        case ThemeConfig::Theme10:
            return false;
        default:
            return true;
    }
}

LoopbackEnabled::LoopbackEnabled(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getLoopbackEnabled<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getLoopbackEnabled<Mode::Visualizer>(themeConfig);
        break;
    }
}
