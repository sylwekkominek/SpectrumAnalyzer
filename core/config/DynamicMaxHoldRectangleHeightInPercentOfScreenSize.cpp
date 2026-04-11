#include "DynamicMaxHoldRectangleHeightInPercentOfScreenSize.hpp"

DynamicMaxHoldRectangleHeightInPercentOfScreenSize::DynamicMaxHoldRectangleHeightInPercentOfScreenSize(float value) : value(value)
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

template<>
float DynamicMaxHoldRectangleHeightInPercentOfScreenSize::getDynamicMaxHoldRectangleHeightInPercentOfScreenSize<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 0.8;
    }
}

template<>
float DynamicMaxHoldRectangleHeightInPercentOfScreenSize::getDynamicMaxHoldRectangleHeightInPercentOfScreenSize<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 0.8;
    }
}

template<>
float DynamicMaxHoldRectangleHeightInPercentOfScreenSize::getDynamicMaxHoldRectangleHeightInPercentOfScreenSize<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 0.8;
    }
}

DynamicMaxHoldRectangleHeightInPercentOfScreenSize::DynamicMaxHoldRectangleHeightInPercentOfScreenSize(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
        case Mode::Analyzer:
            value = getDynamicMaxHoldRectangleHeightInPercentOfScreenSize<Mode::Analyzer>(themeConfig);
            break;
        case Mode::Visualizer:
            value = getDynamicMaxHoldRectangleHeightInPercentOfScreenSize<Mode::Visualizer>(themeConfig);
            break;
        case Mode::StereoRmsMeter:
            value = getDynamicMaxHoldRectangleHeightInPercentOfScreenSize<Mode::StereoRmsMeter>(themeConfig);
            break;
    }
}
