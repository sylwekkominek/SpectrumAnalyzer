#include "MaximizedWindowSize.hpp"

MaximizedWindowSize::MaximizedWindowSize(std::pair<uint16_t, uint16_t> value) : value(value)
{
}

std::string MaximizedWindowSize::getInfo()
{
    return std::string(
        R"(//Description: The user can select the fullscreen resolution. This is particularly useful on devices like the Raspberry Pi, where the maximum resolution may cause performance issues. Allowing users to lower the resolution can improve application responsiveness and ensure smoother performance.
//Default value: 1920×1080
)");
}

std::ostream& operator<<(std::ostream& os, const MaximizedWindowSize &maximizedWindowSize)
{
    os <<"maximizedWindowSize: "<<maximizedWindowSize.value.first<<" "<<maximizedWindowSize.value.second<<std::endl;
    return os;
}

template<>
std::pair<uint16_t, uint16_t> MaximizedWindowSize::getMaximizedWindowSize<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {1920, 1080};
    }
}

template<>
std::pair<uint16_t, uint16_t> MaximizedWindowSize::getMaximizedWindowSize<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {1920, 1080};
    }
}

template<>
std::pair<uint16_t, uint16_t> MaximizedWindowSize::getMaximizedWindowSize<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {1920, 1080};
    }
}

MaximizedWindowSize::MaximizedWindowSize(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getMaximizedWindowSize<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getMaximizedWindowSize<Mode::Visualizer>(themeConfig);
        break;
    case Mode::StereoRmsMeter:
        value = getMaximizedWindowSize<Mode::StereoRmsMeter>(themeConfig);
        break;
    }
}
