#include "DynamicMaxHoldSecondaryVisibilityState.hpp"


DynamicMaxHoldSecondaryVisibilityState::DynamicMaxHoldSecondaryVisibilityState(bool value) : value(value)
{
}

DynamicMaxHoldSecondaryVisibilityState::DynamicMaxHoldSecondaryVisibilityState(const ThemeConfig themeConfig) : value(getDynamicMaxHoldSecondaryVisibilityState(themeConfig))
{
}

std::string DynamicMaxHoldSecondaryVisibilityState::getInfo()
{
    return std::string(
        R"(//Description: Toggles the visibility of secondary on-screen elements that display the peak (max hold) values over time. When enabled, secondary markers show the highest recent signal levels.
//Default value: True
)");
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldSecondaryVisibilityState &dynamicMaxHoldSecondaryVisibilityState)
{
    os <<"dynamicMaxHoldSecondaryVisibilityState: "<<dynamicMaxHoldSecondaryVisibilityState.value<<std::endl;
    return os;
}

bool DynamicMaxHoldSecondaryVisibilityState::getDynamicMaxHoldSecondaryVisibilityState(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return true;
    }
}
