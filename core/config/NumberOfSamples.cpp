#include "NumberOfSamples.hpp"

NumberOfSamples::NumberOfSamples(uint32_t value) : value(value)
{
}

std::string NumberOfSamples::getInfo()
{
    return std::string(
        R"(//Description: Users can adjust the number of audio samples processed each time to find the best setup for them. Using fewer than 4096 samples makes the display smoother and more responsive but lowers the detail at low frequencies. For example, with fewer samples, it becomes harder to distinguish between very close frequencies like 20 Hz and 30 Hz. Increasing the number of samples improves frequency detail but can make the program less responsive.
//Default value: 4096
)");
}

std::ostream& operator<<(std::ostream& os, const NumberOfSamples &numberOfSamples)
{
    os <<"numberOfSamples: "<<numberOfSamples.value<<std::endl;
    return os;
}

template<>
uint32_t NumberOfSamples::getNumberOfSamples<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return 8192;
        case ThemeConfig::Theme2:
            return 2048;
        case ThemeConfig::Theme3:
            return 2048;
        case ThemeConfig::Theme4:
            return 4096;
        case ThemeConfig::Theme5:
            return 8192;
        case ThemeConfig::Theme6:
            return 8192;
        case ThemeConfig::Theme7:
            return 16384;
        case ThemeConfig::Theme8:
            return 8192;
        case ThemeConfig::Theme9:
            return 2048;
        case ThemeConfig::Theme10:
            return 2048;
        default:
            return 4096;
    }
}

template<>
uint32_t NumberOfSamples::getNumberOfSamples<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    return 8192;
}

NumberOfSamples::NumberOfSamples(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getNumberOfSamples<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getNumberOfSamples<Mode::Visualizer>(themeConfig);
        break;
    }
}

