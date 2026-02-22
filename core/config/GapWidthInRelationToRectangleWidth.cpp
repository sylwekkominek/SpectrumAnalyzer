#include "GapWidthInRelationToRectangleWidth.hpp"

GapWidthInRelationToRectangleWidth::GapWidthInRelationToRectangleWidth(float value) : value(value)
{
}

GapWidthInRelationToRectangleWidth::GapWidthInRelationToRectangleWidth(const ThemeConfig themeConfig) : value(getGapWidthInRelationToRectangleWidth(themeConfig))
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

float GapWidthInRelationToRectangleWidth::getGapWidthInRelationToRectangleWidth(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return 0;
    }
}
