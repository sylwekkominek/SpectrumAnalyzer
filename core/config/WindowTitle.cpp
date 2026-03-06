#include "WindowTitle.hpp"


WindowTitle::WindowTitle(const std::string &value) : value(value)
{
}

std::string WindowTitle::getInfo()
{
    return std::string(
        R"(//Description: This file defines the text displayed in the window title bar in order to provide additional runtime information.
)");
}

template<>
std::string WindowTitle::getWindowTitle<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return "( Mode: Loopback    FFT: 8192    Fs: 48kHz    Log Scale: 20Hz – 16kHz )";
        case ThemeConfig::Theme2:
            return  "( Mode: Loopback    FFT: 2048    Fs: 48kHz    Linear Scale: 20Hz – 20kHz )";
        case ThemeConfig::Theme3:
            return  "( Mode: Loopback    FFT: 2048    Fs: 48kHz    Linear Scale: 20Hz – 16kHz )";
        case ThemeConfig::Theme4:
            return  "( Mode: Loopback    FFT: 4096    Fs: 48kHz    Linear Scale: 20Hz – 10kHz )";
        case ThemeConfig::Theme5:
            return  "( Mode: Loopback    FFT: 8192    Fs: 48kHz    Linear Scale: 20Hz – 5kHz )";
        case ThemeConfig::Theme6:
            return  "( Mode: Loopback    FFT: 8192    Fs: 48kHz    Linear Scale: 20Hz – 2kHz )";
        case ThemeConfig::Theme7:
            return  "( Mode: Loopback    FFT: 16384    Fs: 48kHz    Linear Scale: 20Hz – 500Hz )";
        case ThemeConfig::Theme8:
            return "( Mode: Input    FFT: 8192    Fs: 48kHz    Log Scale: 20Hz – 16kHz )";
        case ThemeConfig::Theme9:
            return  "( Mode: Input    FFT: 2048    Fs: 48kHz    Linear Scale: 20Hz – 20kHz )";
        case ThemeConfig::Theme10:
            return  "( Mode: Input    FFT: 2048    Fs: 48kHz    Linear Scale: 20Hz – 16kHz )";

        default:
            return  "";
    }
}

template<>
std::string WindowTitle::getWindowTitle<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return "( Mode: Loopback )";
        case ThemeConfig::Theme2:
            return "( Mode: Loopback )";
        case ThemeConfig::Theme3:
            return "( Mode: Loopback )";
        case ThemeConfig::Theme4:
            return "( Mode: Loopback )";
        case ThemeConfig::Theme5:
            return "( Mode: Loopback )";
        case ThemeConfig::Theme6:
            return "( Mode: Loopback )";
        case ThemeConfig::Theme7:
            return "( Mode: Loopback )";
        case ThemeConfig::Theme8:
            return "( Mode: Input )";
        case ThemeConfig::Theme9:
            return "( Mode: Input )";
        case ThemeConfig::Theme10:
            return "( Mode: Input )";

        default:
            return  "";
    }
}

WindowTitle::WindowTitle(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getWindowTitle<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getWindowTitle<Mode::Visualizer>(themeConfig);
        break;
    }
}
