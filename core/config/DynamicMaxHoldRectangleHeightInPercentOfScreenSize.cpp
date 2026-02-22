#include "DynamicMaxHoldRectangleHeightInPercentOfScreenSize.hpp"

DynamicMaxHoldRectangleHeightInPercentOfScreenSize::DynamicMaxHoldRectangleHeightInPercentOfScreenSize(float value) : value(value)
{
}

DynamicMaxHoldRectangleHeightInPercentOfScreenSize::DynamicMaxHoldRectangleHeightInPercentOfScreenSize(const ThemeConfig themeConfig) : value(getDynamicMaxHoldRectangleHeightInPercentOfScreenSize(themeConfig))
{
}

std::string DynamicMaxHoldRectangleHeightInPercentOfScreenSize::getInfo()
{
    return std::string(
        R"(//Description: Allows the user to control the height of the rectangles that represent dynamic max hold values. The value is given as a percentage of the screen height — for example, 1 means the rectangle will be 1% of the total screen height.
//Default value: 0.8
)");
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldRectangleHeightInPercentOfScreenSize &dynamicMaxHoldRectangleHeightInPercentOfScreenSize)
{
    os <<"dynamicMaxHoldRectangleHeightInPercentOfScreenSize: "<<dynamicMaxHoldRectangleHeightInPercentOfScreenSize.value<<std::endl;
    return os;
}

float DynamicMaxHoldRectangleHeightInPercentOfScreenSize::getDynamicMaxHoldRectangleHeightInPercentOfScreenSize(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 0.8;
    }
}
