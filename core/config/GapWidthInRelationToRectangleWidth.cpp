#include "GapWidthInRelationToRectangleWidth.hpp"

GapWidthInRelationToRectangleWidth::GapWidthInRelationToRectangleWidth(float value) : value(value)
{
}

std::string GapWidthInRelationToRectangleWidth::getInfo()
{
    return std::string();
}

std::ostream& operator<<(std::ostream& os, const GapWidthInRelationToRectangleWidth& gapWidthInRelationToRectangleWidth)
{
    os <<"gapWidthInRelationToRectangleWidth: "<<gapWidthInRelationToRectangleWidth.value<<std::endl;
    return os;
}

template<>
float GapWidthInRelationToRectangleWidth::getGapWidthInRelationToRectangleWidth<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 0;
    }
}

template<>
float GapWidthInRelationToRectangleWidth::getGapWidthInRelationToRectangleWidth<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme2:
        return 1.0;
        default:
            return 0;
    }
}

template<>
float GapWidthInRelationToRectangleWidth::getGapWidthInRelationToRectangleWidth<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return 1.0;
        case ThemeConfig::Theme2:
            return 1.0;
        default:
            return 0;
    }
}


GapWidthInRelationToRectangleWidth::GapWidthInRelationToRectangleWidth(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getGapWidthInRelationToRectangleWidth<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getGapWidthInRelationToRectangleWidth<Mode::Visualizer>(themeConfig);
        break;
    case Mode::StereoRmsMeter:
        value = getGapWidthInRelationToRectangleWidth<Mode::StereoRmsMeter>(themeConfig);
        break;
    }
}
