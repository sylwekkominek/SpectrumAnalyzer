#include "ColorsOfRectangle.hpp"


ColorsOfRectangle::ColorsOfRectangle(const ColorsOfRectanglePerVertices &value) : value(value)
{
}

ColorsOfRectangle::ColorsOfRectangle(const ThemeConfig themeConfig) : value(getColorsOfRectangle(themeConfig))
{
}

std::string ColorsOfRectangle::getInfo()
{
    return std::string(
        R"(//Description: Each bar (rectangle) on the screen is made up of 4 corners (vertices). You can set a separate color for each vertex using RGBA values — where R is red, G is green, B is blue, and A stands for transparency. The GPU then smoothly blends the colors between the corners, allowing you to easily control the appearance of each bar, for example by creating smooth color gradients from top to bottom.
//Default value: RGBA color (Red, Green, Blue, Transparency) for each vertex
)");
}

std::ostream& operator<<(std::ostream& os, const ColorsOfRectangle &colorsOfRectangle)
{
    os <<"colorsOfRectangle: "<<std::endl;
    for(auto &[vertex, color]: colorsOfRectangle.value)
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

ColorsOfRectanglePerVertices ColorsOfRectangle::getColorsOfRectangle(const ThemeConfig themeConfig)
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
