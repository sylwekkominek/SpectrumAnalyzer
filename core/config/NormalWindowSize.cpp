#include "NormalWindowSize.hpp"

NormalWindowSize::NormalWindowSize(std::pair<uint16_t, uint16_t> value) : value(value)
{
}

std::string NormalWindowSize::getInfo()
{
    return std::string(
        R"(//Description: The initial window size when the application starts in windowed mode. This setting ensures that the strips always look the way the user wants, providing a consistent visual layout without the need for manual resizing after launch.
//Default value: 1280×512
)");
}

std::ostream& operator<<(std::ostream& os, const NormalWindowSize &normalWindowSize)
{
    os <<"maximizedWindowSize: "<<normalWindowSize.value.first<<" "<<normalWindowSize.value.second<<std::endl;
    return os;
}

template<>
std::pair<uint16_t, uint16_t> NormalWindowSize::getNormalWindowSize<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {1280, 512};
    }
}

template<>
std::pair<uint16_t, uint16_t> NormalWindowSize::getNormalWindowSize<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {1280, 512};
    }
}

template<>
std::pair<uint16_t, uint16_t> NormalWindowSize::getNormalWindowSize<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme4:
            return {256, 920};
        case ThemeConfig::Theme8:
            return {104, 800};
        case ThemeConfig::Theme9:
            return {104, 800};
        default:
            return {256, 768};

    }
}

NormalWindowSize::NormalWindowSize(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getNormalWindowSize<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getNormalWindowSize<Mode::Visualizer>(themeConfig);
        break;
    case Mode::StereoRmsMeter:
        value = getNormalWindowSize<Mode::StereoRmsMeter>(themeConfig);
        break;
    }
}
