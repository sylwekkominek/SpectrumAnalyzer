#include "MaxQueueSize.hpp"


MaxQueueSize::MaxQueueSize(uint32_t value) : value(value)
{
}

std::string MaxQueueSize::getInfo()
{
    return std::string(
        R"()");
}

std::ostream& operator<<(std::ostream& os, const MaxQueueSize &maxQueueSize)
{
    os <<"maxQueueSize: "<<maxQueueSize.value<<std::endl;
    return os;
}

template<>
uint32_t MaxQueueSize::getMaxQueueSize<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 10;
    }
}

template<>
uint32_t MaxQueueSize::getMaxQueueSize<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 10;
    }
}

template<>
uint32_t MaxQueueSize::getMaxQueueSize<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 10;
    }
}

MaxQueueSize::MaxQueueSize(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getMaxQueueSize<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getMaxQueueSize<Mode::Visualizer>(themeConfig);
        break;
    case Mode::StereoRmsMeter:
        value = getMaxQueueSize<Mode::StereoRmsMeter>(themeConfig);
        break;
    }
}
