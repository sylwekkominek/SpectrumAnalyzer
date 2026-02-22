#include "NumberOfSignalsForMaxHold.hpp"

NumberOfSignalsForMaxHold::NumberOfSignalsForMaxHold(uint32_t value) : value(value)
{
}

NumberOfSignalsForMaxHold::NumberOfSignalsForMaxHold(const ThemeConfig themeConfig) : value(getNumberOfSignalsForMaxHold(themeConfig))
{
}

std::string NumberOfSignalsForMaxHold::getInfo()
{
    return std::string(
        R"(//Description: Defines how many of the recent signal frames are considered when calculating the maximum hold value. This affects how quickly the displayed peak values update and start to drop, making the peak indicators appear either more stable or more dynamic.
//Default value: 5
)");
}

std::ostream& operator<<(std::ostream& os, const NumberOfSignalsForMaxHold &numberOfSignalsForMaxHold)
{
    os <<"numberOfSignalsForMaxHold: "<<numberOfSignalsForMaxHold.value<<std::endl;
    return os;
}

uint32_t NumberOfSignalsForMaxHold::getNumberOfSignalsForMaxHold(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme7:
            return 2;
        default:
            return 5;
    }
}
