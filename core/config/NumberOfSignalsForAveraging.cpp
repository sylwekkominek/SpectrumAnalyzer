#include "NumberOfSignalsForAveraging.hpp"

NumberOfSignalsForAveraging::NumberOfSignalsForAveraging(uint32_t value) : value(value)
{
}

NumberOfSignalsForAveraging::NumberOfSignalsForAveraging(const ThemeConfig themeConfig) : value(getNumberOfSignalsForAveraging(themeConfig))
{
}

std::string NumberOfSignalsForAveraging::getInfo()
{
    return std::string(
        R"(//Description: This parameter controls how many signal samples are averaged together to produce the displayed output. Increasing the number of signals for averaging results in smoother, more stable visualizations by reducing short-term fluctuations. However, it also introduces a slight delay in responsiveness. Lower values make the output more immediate and reactive but can appear more “jumpy.”
//Default value: 1
)");
}

std::ostream& operator<<(std::ostream& os, const NumberOfSignalsForAveraging &numberOfSignalsForAveraging)
{
    os <<"numberOfSignalsForAveraging: "<<numberOfSignalsForAveraging.value<<std::endl;
    return os;
}

uint32_t NumberOfSignalsForAveraging::getNumberOfSignalsForAveraging(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme7:
            return 2;
        default:
            return 1;
    }
}
