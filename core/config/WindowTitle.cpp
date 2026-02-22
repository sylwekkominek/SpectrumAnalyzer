#include "WindowTitle.hpp"



WindowTitle::WindowTitle(const std::string &value) : value(value)
{
}

WindowTitle::WindowTitle(const ThemeConfig themeConfig) : value(getWindowTitle(themeConfig))
{
}

std::string WindowTitle::getInfo()
{
    return std::string(
        R"(//Description: This file defines the text displayed in the window title bar in order to provide additional runtime information.
)");
}

std::string WindowTitle::getWindowTitle(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return "( FFT: 8192    Fs: 48kHz   Log Scale: 20Hz – 16kHz )";
        case ThemeConfig::Theme2:
            return  "( FFT: 2048    Fs: 48kHz   Linear Scale: 20Hz – 20kHz )";
        case ThemeConfig::Theme3:
            return  "( FFT: 2048    Fs: 48kHz   Linear Scale: 20Hz – 16kHz )";
        case ThemeConfig::Theme4:
            return  "( FFT: 4096    Fs: 48kHz   Linear Scale: 20Hz – 10kHz )";
        case ThemeConfig::Theme5:
            return  "( FFT: 8192    Fs: 48kHz   Linear Scale: 20Hz – 5kHz )";
        case ThemeConfig::Theme6:
            return  "( FFT: 8192    Fs: 48kHz   Linear Scale: 20Hz – 2kHz )";
        case ThemeConfig::Theme7:
            return  "( FFT: 16384    Fs: 48kHz   Linear Scale: 20Hz – 500kHz )";
        default:
            return  "";
    }
}
