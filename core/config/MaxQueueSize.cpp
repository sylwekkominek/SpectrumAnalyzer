#include "MaxQueueSize.hpp"


MaxQueueSize::MaxQueueSize(uint32_t value) : value(value)
{
}

MaxQueueSize::MaxQueueSize(const ThemeConfig themeConfig) : value(getMaxQueueSize(themeConfig))
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

uint32_t MaxQueueSize::getMaxQueueSize(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 20;
    }
}
