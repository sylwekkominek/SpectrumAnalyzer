#include "NumberOfSignalsForMaxHold.hpp"

NumberOfSignalsForMaxHold::NumberOfSignalsForMaxHold(uint32_t value) : value(value)
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

template<>
uint32_t NumberOfSignalsForMaxHold::getNumberOfSignalsForMaxHold<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme7:
            return 2;
        default:
            return 5;
    }
}

template<>
uint32_t NumberOfSignalsForMaxHold::getNumberOfSignalsForMaxHold<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    return 5;
}

NumberOfSignalsForMaxHold::NumberOfSignalsForMaxHold(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getNumberOfSignalsForMaxHold<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getNumberOfSignalsForMaxHold<Mode::Visualizer>(themeConfig);
        break;
    }
}
