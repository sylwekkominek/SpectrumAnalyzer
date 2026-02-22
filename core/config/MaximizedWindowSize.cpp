#include "MaximizedWindowSize.hpp"

MaximizedWindowSize::MaximizedWindowSize(std::pair<uint16_t, uint16_t> value) : value(value)
{
}

MaximizedWindowSize::MaximizedWindowSize(const ThemeConfig themeConfig) : value(getMaximizedWindowSize(themeConfig))
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

std::pair<uint16_t, uint16_t> MaximizedWindowSize::getMaximizedWindowSize(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {1920, 1080};
    }
}
