#include "OffsetFactor.hpp"

OffsetFactor::OffsetFactor(float value) : value(value)
{
}

std::string OffsetFactor::getInfo()
{
    return std::string(
        R"(//Description: Allows precise adjustment of the signal amplitude, useful when working with external devices or simply to raise the signal level if the displayed bars on the screen appear too low.
//Default value: 0
)");
}

std::ostream& operator<<(std::ostream& os, const OffsetFactor &offsetFactor)
{
    os <<"offsetFactor: "<<offsetFactor.value<<std::endl;
    return os;
}

template<>
float OffsetFactor::getOffsetFactor<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 0;
    }
}

template<>
float OffsetFactor::getOffsetFactor<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 0;
    }
}

OffsetFactor::OffsetFactor(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getOffsetFactor<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getOffsetFactor<Mode::Visualizer>(themeConfig);
        break;
    }
}
