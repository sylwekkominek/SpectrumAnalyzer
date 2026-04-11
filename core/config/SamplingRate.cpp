#include "SamplingRate.hpp"

SamplingRate::SamplingRate(uint32_t value) : value(value)
{
}

std::string SamplingRate::getInfo()
{
    return std::string(
        R"(//Description: This parameter is rarely changed but can be useful when analyzing audio at non-standard sample rates. It also allows experimenting with oversampling or downsampling (which PyAudio supports). Additionally, you might need to adjust it when connecting external devices with different sampling frequencies.
//Default value: 48000 Hz
)");
}

std::ostream& operator<<(std::ostream& os, const SamplingRate &samplingRate)
{
    os <<"samplingRate: "<<samplingRate.value<<std::endl;
    return os;
}

template<>
uint32_t SamplingRate::getSamplingRate<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 48000;
    }
}

template<>
uint32_t SamplingRate::getSamplingRate<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 48000;
    }
}

template<>
uint32_t SamplingRate::getSamplingRate<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
    default:
        return 48000;
    }
}

SamplingRate::SamplingRate(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getSamplingRate<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getSamplingRate<Mode::Visualizer>(themeConfig);
        break;
    case Mode::StereoRmsMeter:
        value = getSamplingRate<Mode::StereoRmsMeter>(themeConfig);
        break;
    }
}
