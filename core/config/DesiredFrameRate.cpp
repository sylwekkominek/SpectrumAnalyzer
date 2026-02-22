#include "DesiredFrameRate.hpp"

DesiredFrameRate::DesiredFrameRate(uint32_t value) : value(value)
{
}

DesiredFrameRate::DesiredFrameRate(const ThemeConfig themeConfig) : value(getDesiredFrameRate(themeConfig))
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

uint32_t DesiredFrameRate::getDesiredFrameRate(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme7:
            return 40;

        default:
            return 55;
    }
}
