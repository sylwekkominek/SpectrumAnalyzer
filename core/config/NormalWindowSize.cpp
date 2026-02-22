#include "NormalWindowSize.hpp"

NormalWindowSize::NormalWindowSize(std::pair<uint16_t, uint16_t> value) : value(value)
{
}

NormalWindowSize::NormalWindowSize(const ThemeConfig themeConfig) : value(getNormalWindowSize(themeConfig))
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

std::pair<uint16_t, uint16_t> NormalWindowSize::getNormalWindowSize(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {1280, 512};
    }
}
