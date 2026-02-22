#include "DynamicMaxHoldAccelerationStateOfFalling.hpp"

DynamicMaxHoldAccelerationStateOfFalling::DynamicMaxHoldAccelerationStateOfFalling(float value) : value(value)
{
}

DynamicMaxHoldAccelerationStateOfFalling::DynamicMaxHoldAccelerationStateOfFalling(const ThemeConfig themeConfig) : value(getDynamicMaxHoldAccelerationStateOfFalling(themeConfig))
{
}

std::string DynamicMaxHoldAccelerationStateOfFalling::getInfo()
{
    return std::string(
        R"(//Description: Determines whether the max hold elements fall with acceleration or at a constant speed. This gives the user control over the visual behavior and final effect of the peak indicators.
//Default value: True
)");
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldAccelerationStateOfFalling & dynamicMaxHoldAccelerationStateOfFalling)
{
    os <<"dynamicMaxHoldAccelerationStateOfFalling: "<<dynamicMaxHoldAccelerationStateOfFalling.value<<std::endl;
    return os;
}

bool DynamicMaxHoldAccelerationStateOfFalling::getDynamicMaxHoldAccelerationStateOfFalling(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return true;
    }
}
