#include "VerticalDbfsRange.hpp"

VerticalDbfsRange::VerticalDbfsRange(std::pair<float, float> value) : value(value)
{
}

std::string VerticalDbfsRange::getInfo()
{
    return std::string(
        R"(// Description: Returns the dBFS range used for the vertical axis of the chart.
// The range is defined in decibels relative to full scale (dBFS), typically from 0 dBFS
// (maximum signal level) down to a negative value (e.g., -96 dBFS) representing silence.
// These bounds determine how signal amplitude is mapped and displayed on the vertical axis.
)");
}

std::ostream& operator<<(std::ostream& os, const VerticalDbfsRange &verticalDbfsRange)
{
    os <<"verticalDbfsRange: "<<verticalDbfsRange.value.first<<" "<<verticalDbfsRange.value.second<<std::endl;
    return os;
}

template<>
std::pair<float, float> VerticalDbfsRange::getVerticalDbfsRange<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    return {-96.32, 0};
}

template<>
std::pair<float, float> VerticalDbfsRange::getVerticalDbfsRange<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    return {-96.32, 0};
}

template<>
std::pair<float, float> VerticalDbfsRange::getVerticalDbfsRange<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    return {-60, 0};
}


VerticalDbfsRange::VerticalDbfsRange(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
        case Mode::Analyzer:
            value = getVerticalDbfsRange<Mode::Analyzer>(themeConfig);
            break;
        case Mode::Visualizer:
            value = getVerticalDbfsRange<Mode::Visualizer>(themeConfig);
            break;
        case Mode::StereoRmsMeter:
            value = getVerticalDbfsRange<Mode::StereoRmsMeter>(themeConfig);
            break;
    }
}

