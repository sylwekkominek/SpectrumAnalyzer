#include "HorizontalDrawingArea.hpp"

HorizontalDrawingArea::HorizontalDrawingArea(std::pair<float, float> value) : value(value)
{
}

std::string HorizontalDrawingArea::getInfo()
{
    return std::string(
        R"(// |-- 5% margin --|======== 90% plotting area ========|-- 5% margin --|
//
// This setting consists of two values:
// 1) Margin (gap) percentage
// 2) Plotting area percentage
//
// The margin defines how much empty space is left from both sides of the window before the graph starts rendering.
// The plotting area defines how much of the total window width is used for drawing the graph.
)");
}

std::ostream& operator<<(std::ostream& os, const HorizontalDrawingArea &horizontalDrawingArea)
{

    os <<"horizontalDrawingArea: "<<horizontalDrawingArea.value.first<<" "<<horizontalDrawingArea.value.second<<std::endl;
    os <<std::endl;

    return os;
}

template<>
std::pair<float, float> HorizontalDrawingArea::getHorizontalDrawingArea<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    return {5, 90};
}

template<>
std::pair<float, float> HorizontalDrawingArea::getHorizontalDrawingArea<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    return {5, 90};
}

template<>
std::pair<float, float> HorizontalDrawingArea::getHorizontalDrawingArea<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return {10,80};
        case ThemeConfig::Theme2:
            return {10,80};
        default:
            return {0,100};
    }
}


HorizontalDrawingArea::HorizontalDrawingArea(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
        case Mode::Analyzer:
            value = getHorizontalDrawingArea<Mode::Analyzer>(themeConfig);
            break;
        case Mode::Visualizer:
            value = getHorizontalDrawingArea<Mode::Visualizer>(themeConfig);
            break;
        case Mode::StereoRmsMeter:
            value = getHorizontalDrawingArea<Mode::StereoRmsMeter>(themeConfig);
            break;
    }
}

