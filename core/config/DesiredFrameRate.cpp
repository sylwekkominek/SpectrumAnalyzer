#include "DesiredFrameRate.hpp"

DesiredFrameRate::DesiredFrameRate(uint32_t value) : value(value)
{
}

std::string DesiredFrameRate::getInfo()
{
    return std::string(
        R"(//Description: Sets the target number of frames per second (FPS) displayed on the screen. Since the number of samples and sampling rate limit how frequently new data can be processed, the program increases the frame rate by using overlapping. For example, with 4096 samples at a 44100 Hz sampling rate, the default frame rate is about 11 FPS. To reach higher rates like 60 FPS, the program applies Welch’s method, processing overlapping segments of the signal to effectively analyze more data per second. This parameter also allows adjusting the application to run smoothly on hardware with limited performance, such as a Raspberry Pi, which may struggle with higher frame rates, especially at higher resolutions.
//Default value: 55 FPS
)");
}

std::ostream& operator<<(std::ostream& os, const DesiredFrameRate &desiredFrameRate)
{
    os <<"desiredFrameRate: "<<desiredFrameRate.value<<std::endl;
    return os;
}

template<>
uint32_t DesiredFrameRate::getDesiredFrameRate<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme7:
            return 40;

        default:
            return 55;
    }
}

template<>
uint32_t DesiredFrameRate::getDesiredFrameRate<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 55;
    }
}

DesiredFrameRate::DesiredFrameRate(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getDesiredFrameRate<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getDesiredFrameRate<Mode::Visualizer>(themeConfig);
        break;
    }
}
