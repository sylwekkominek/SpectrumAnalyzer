#include "OffsetFactor.hpp"

OffsetFactor::OffsetFactor(float value) : value(value)
{
}

OffsetFactor::OffsetFactor(const ThemeConfig themeConfig) : value(getOffsetFactor(themeConfig))
{
}

std::string OffsetFactor::getInfo()
{
    return std::string(
        R"(//Description: Allows precise adjustment of the signal amplitude, useful when working with external devices or simply to raise the signal level if the displayed bars on the screen appear too low.
//Default value: 0
)");
}

std::ostream& operator<<(std::ostream& os, const OffsetFactor &offsetFactor)
{
    os <<"offsetFactor: "<<offsetFactor.value<<std::endl;
    return os;
}

float OffsetFactor::getOffsetFactor(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 0;
    }
}
