#include "AlphaFactor.hpp"

AlphaFactor::AlphaFactor(float value) : value(value)
{
}

AlphaFactor::AlphaFactor(const ThemeConfig themeConfig) : value(getAlphaFactor(themeConfig))
{
}

std::string AlphaFactor::getInfo()
{
    return std::string(
        R"(//Description: Controls the amount of smoothing applied to the signal visualization. A lower alpha factor results in a smoother and more stable display, but with slower response to rapid changes. A higher alpha makes the visualization more responsive and sensitive to signal variations, although it may result in a more flickery or unstable display.
//Default value: 0.25
)");
}

std::ostream& operator<<(std::ostream& os, const AlphaFactor &alphaFactor)
{
    os <<"alphaFactor: "<<alphaFactor.value<<std::endl;
    return os;
}


float AlphaFactor::getAlphaFactor(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme7:
            return 0.70f;

        default:
            return 0.25f;
    }
}
