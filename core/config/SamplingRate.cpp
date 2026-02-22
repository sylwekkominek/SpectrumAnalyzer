#include "SamplingRate.hpp"

SamplingRate::SamplingRate(uint32_t value) : value(value)
{
}

SamplingRate::SamplingRate(const ThemeConfig themeConfig) : value(getSamplingRate(themeConfig))
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

uint32_t SamplingRate::getSamplingRate(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 48000;
    }
}
