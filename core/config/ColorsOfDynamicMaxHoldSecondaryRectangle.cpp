#include "ColorsOfDynamicMaxHoldSecondaryRectangle.hpp"


ColorsOfDynamicMaxHoldSecondaryRectangle::ColorsOfDynamicMaxHoldSecondaryRectangle(const ColorsOfRectanglePerVertices &value) : value(value)
{
}

std::string ColorsOfDynamicMaxHoldSecondaryRectangle::getInfo()
{
    return std::string(
        R"(//Description: Uses the same method of assigning RGBA colors to each of the 4 rectangle vertices as for the main bars. This time, however, it applies to the secondary visual elements that hold and display the peak (max hold) values over time. By setting the vertex colors, you can control the appearance of these peak indicators, including gradients and transparency.
//Default value: RGBA color (Red, Green, Blue, Transparency) for each vertex)");
}

template<>
ColorsOfRectanglePerVertices ColorsOfDynamicMaxHoldSecondaryRectangle::getColorsOfDynamicMaxHoldSecondaryRectangle<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    const ColorsOfRectanglePerVertices value{
        {0,{0.20, 0.20, 0.20, 0.25}},
        {1,{0.00, 0.00, 0.00, 0.25}},
        {2,{0.00, 0.00, 0.00, 0.25}},
        {3,{0.20, 0.20, 0.20, 0.25}}
    };


    const ColorsOfRectanglePerVertices defaultValue{
        {0,{0.7, 0.7, 0.7, 0.25}},
        {1,{0.7, 0.7, 0.7, 0.25}},
        {2,{0.7, 0.7, 0.7, 0.25}},
        {3,{0.7, 0.7, 0.7, 0.25}}
    };

    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return value;
        case ThemeConfig::Theme8:
            return value;
        default:
            return defaultValue;
    }
}

template<>
ColorsOfRectanglePerVertices ColorsOfDynamicMaxHoldSecondaryRectangle::getColorsOfDynamicMaxHoldSecondaryRectangle<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    const ColorsOfRectanglePerVertices value{
        {0,{0.20, 0.20, 0.20, 0.25}},
        {1,{0.00, 0.00, 0.00, 0.25}},
        {2,{0.00, 0.00, 0.00, 0.25}},
        {3,{0.20, 0.20, 0.20, 0.25}}
    };

    return value;
}

template<>
ColorsOfRectanglePerVertices ColorsOfDynamicMaxHoldSecondaryRectangle::getColorsOfDynamicMaxHoldSecondaryRectangle<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    const ColorsOfRectanglePerVertices value{
        {0,{0.50, 0.50, 0.50, 0.4}},
        {1,{0.00, 0.00, 0.00, 0.4}},
        {2,{0.00, 0.00, 0.00, 0.4}},
        {3,{0.50, 0.50, 0.50, 0.4}}
    };

    return value;
}


ColorsOfDynamicMaxHoldSecondaryRectangle::ColorsOfDynamicMaxHoldSecondaryRectangle(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getColorsOfDynamicMaxHoldSecondaryRectangle<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getColorsOfDynamicMaxHoldSecondaryRectangle<Mode::Visualizer>(themeConfig);
        break;
    case Mode::StereoRmsMeter:
        value = getColorsOfDynamicMaxHoldSecondaryRectangle<Mode::StereoRmsMeter>(themeConfig);
        break;
    }
}

