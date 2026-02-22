#include "HorizontalLinePositions.hpp"


HorizontalLinePositions::HorizontalLinePositions(const Positions &value) : value(value)
{
}

HorizontalLinePositions::HorizontalLinePositions(const ThemeConfig themeConfig) : value(getHorizontalLinePositions(themeConfig))
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

Positions HorizontalLinePositions::getHorizontalLinePositions(const ThemeConfig themeConfig)
{
    const Positions defaultValue{-6.02, -12.04, -18.06, -24.08, -30.10, -36.12, -36.12, -42.14, -48.16, -54.18, -60.20, -66.22, -72.24, -78.26, -84.28, -90.30};

    switch(themeConfig)
    {
        default:
            return defaultValue;
    }
}
