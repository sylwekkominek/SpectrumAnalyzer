#include "ColorsOfDynamicMaxHoldRectangle.hpp"

ColorsOfDynamicMaxHoldRectangle::ColorsOfDynamicMaxHoldRectangle(const ColorsOfRectanglePerVertices &value) : value(value)
{
}

std::string ColorsOfDynamicMaxHoldRectangle::getInfo()
{
    return std::string(
        R"(//Description: Uses the same method of assigning RGBA colors to each of the 4 rectangle vertices as for the main bars. This time, however, it applies to the small visual elements that hold and display the peak (max hold) values over time. By setting the vertex colors, you can control the appearance of these peak indicators, including gradients and transparency.
//Default value: RGBA color (Red, Green, Blue, Transparency) for each vertex
)");
}

std::ostream& operator<<(std::ostream& os, const ColorsOfDynamicMaxHoldRectangle &colorsOfDynamicMaxHoldRectangle)
{
    os <<"colorsOfDynamicMaxHoldRectangle: "<<std::endl;
    for(auto &[vertex, color]: colorsOfDynamicMaxHoldRectangle.value)
    {
        os<<"vertex: "<<vertex<<" color: ";
        for(auto & component: color)
        {
            os<<component<<" ";
        }
        os<<std::endl;
    }

    return os;
}

template<>
ColorsOfRectanglePerVertices ColorsOfDynamicMaxHoldRectangle::getColorsOfDynamicMaxHoldRectangle<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    const ColorsOfRectanglePerVertices defaultValue{
        {0,{0.2, 0.2, 0.2,0.1}},
        {1,{0,0,0,0.1}},
        {2,{0,0,0,0.1}},
        {3,{0.2, 0.2, 0.2,0.1}}
    };

    switch(themeConfig)
    {
        default:
            return defaultValue;
    }
}

template<>
ColorsOfRectanglePerVertices ColorsOfDynamicMaxHoldRectangle::getColorsOfDynamicMaxHoldRectangle<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    const ColorsOfRectanglePerVertices defaultValue{
        {0,{0.2, 0.2, 0.2,0.1}},
        {1,{0,0,0,0.1}},
        {2,{0,0,0,0.1}},
        {3,{0.2, 0.2, 0.2,0.1}}
    };

    switch(themeConfig)
    {
        default:
            return defaultValue;
    }
}

ColorsOfDynamicMaxHoldRectangle::ColorsOfDynamicMaxHoldRectangle(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
        case Mode::Analyzer:
            value = getColorsOfDynamicMaxHoldRectangle<Mode::Analyzer>(themeConfig);
            break;
        case Mode::Visualizer:
            value = getColorsOfDynamicMaxHoldRectangle<Mode::Visualizer>(themeConfig);
            break;
    }
}


