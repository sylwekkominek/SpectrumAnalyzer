#include "HorizontalLinePositions.hpp"


HorizontalLinePositions::HorizontalLinePositions(const Positions &value) : value(value)
{
}

std::string HorizontalLinePositions::getInfo()
{
    return std::string(
        R"(//Description: Lets the user define how many horizontal lines are shown on the display and at which signal power levels (in dBFS) they appear. This allows for precise customization of the scale and visual reference points in the signal visualization.
//Default value: –6.02, –12.04, –18.06, ..., –90.30 dBFS
)");
}

std::ostream& operator<<(std::ostream& os, const HorizontalLinePositions &horizontalLinePositions)
{
    os <<"horizontalLinePositions: ";

    for(const auto &el: horizontalLinePositions.value)
    {
        os <<el<<" ";
    }
    os <<std::endl;

    return os;
}

template<>
Positions HorizontalLinePositions::getHorizontalLinePositions<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    const Positions defaultValue{-6.02, -12.04, -18.06, -24.08, -30.10, -36.12, -36.12, -42.14, -48.16, -54.18, -60.20, -66.22, -72.24, -78.26, -84.28, -90.30};

    switch(themeConfig)
    {
        default:
            return defaultValue;
    }
}

template<>
Positions HorizontalLinePositions::getHorizontalLinePositions<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    const Positions defaultValue{-6.02, -12.04, -18.06, -24.08, -30.10, -36.12, -36.12, -42.14, -48.16, -54.18, -60.20, -66.22, -72.24, -78.26, -84.28, -90.30};

    switch(themeConfig)
    {
        default:
            return defaultValue;
    }
}

template<>
Positions HorizontalLinePositions::getHorizontalLinePositions<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    const Positions defaultValue{-6.02, -12.04, -18.06, -24.08, -30.10, -36.12, -36.12, -42.14, -48.16, -54.18};

    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return {-6.02, -12.04, -18.06, -24.08, -30.10, -36.12, -36.12, -42.14, -48.16, -54.18};
        case ThemeConfig::Theme2:
            return {-6.02, -12.04, -18.06, -24.08, -30.10, -36.12, -36.12, -42.14, -48.16, -54.18};
        default:
            return defaultValue;
    }
}

HorizontalLinePositions::HorizontalLinePositions(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getHorizontalLinePositions<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getHorizontalLinePositions<Mode::Visualizer>(themeConfig);
        break;
    case Mode::StereoRmsMeter:
        value = getHorizontalLinePositions<Mode::StereoRmsMeter>(themeConfig);
        break;

    }
}
