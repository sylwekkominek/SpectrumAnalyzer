#include "SingleScaleMode.hpp"

SingleScaleMode::SingleScaleMode(bool value) : value(value)
{
}

std::string SingleScaleMode::getInfo()
{
    return std::string(
        R"(//Description: Enables selection between a single scale placed in the center of the window or two identical scales placed on the sides.)");
}

std::ostream& operator<<(std::ostream& os, const SingleScaleMode &singleScaleMode)
{
    os <<"singleScaleMode: "<<singleScaleMode.value<<std::endl;
    return os;
}

template<>
bool SingleScaleMode::getSingleScaleMode<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    return false;
}

template<>
bool SingleScaleMode::getSingleScaleMode<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    return false;
}

template<>
bool SingleScaleMode::getSingleScaleMode<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme4:
            return false;
        case ThemeConfig::Theme5:
            return false;
        case ThemeConfig::Theme6:
            return false;
        default:
            return true;
    }
}

SingleScaleMode::SingleScaleMode(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
        case Mode::Analyzer:
            value = getSingleScaleMode<Mode::Analyzer>(themeConfig);
            break;
        case Mode::Visualizer:
            value = getSingleScaleMode<Mode::Visualizer>(themeConfig);
            break;
        case Mode::StereoRmsMeter:
            value = getSingleScaleMode<Mode::StereoRmsMeter>(themeConfig);
            break;
    }
}
