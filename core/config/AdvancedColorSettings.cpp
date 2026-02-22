#include "AdvancedColorSettings.hpp"



AdvancedColorSettings::AdvancedColorSettings(const std::string &value) : value(value)
{
}

AdvancedColorSettings::AdvancedColorSettings(const ThemeConfig themeConfig) : value(getAdvancedColorSettings(themeConfig))
{
}

std::string AdvancedColorSettings::getInfo()
{
    return std::string(
        R"(//Description: The application first determines how many frequency bins are defined by the user, based on the number of frequencies provided by the getFrequencies() function. Then, it divides the screen into the same number of vertical bars — one for each frequency. Each bar is built using two triangles (6 vertices), which form a rectangle defined by 4 unique corner points in 2D space (x, y). These bars are then sent to the GPU with their initial positions. At this stage, all bars are evenly spaced and aligned. As the application analyzes the audio (or other signal) in real time, it calculates the power of each frequency. Based on this power, the height or vertical position of each bar is updated dynamically to reflect the current signal strength. The vertex shader updates the positions of the bar's corners depending on the signal’s power for that frequency. The GPU then fills in all the pixels between those corners by generating fragments — and each of these fragments is processed by the fragment shader. One of the values the fragment shader receives is calculatedPosition, which (in simplified terms) tells it how high up the current pixel is within its rectangle. This vertical position is used to control how the pixel will be colored. In addition, the shader receives vertColor values — these are colors assigned to each corner of the rectangle using application-side functions like getColorsOfRectangle() and getColorsOfDynamicMaxHoldRectangle(). The GPU automatically blends these colors across the surface of the rectangle, so each pixel gets its own color depending on where it lies. Inside the fragment shader, this blended color is combined with a vertical color gradient that transitions from blue → cyan → green → yellow → red, depending on the pixel’s height within the bar. This results in vibrant and informative color transitions that follow the signal in real time. Users can adjust the rectangle corner colors directly in the application to experiment with color effects. And because the shader computes colors for each pixel independently, there’s a lot of flexibility for customization. More advanced users can even edit the shader code to fully control how colors behave. All of this runs on the GPU, which executes the fragment shader for every pixel of every bar in parallel. This makes the visualization extremely fast and smooth — even when rendering many bars at once. This method not only results in dynamic and responsive visualizations, but also allows users to experiment and learn how shaders and GPU parallelism work in real-time graphics.
//if you get errors after modification please find following log with error msg: "VS log:"
)");
}

std::string AdvancedColorSettings::getAdvancedColorSettings(const ThemeConfig themeConfig)
{

    const std::string str1 = R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform vec2 boundary;
uniform uint themeNumber;
vec3 defaultNebulaBarColor(float y) {
    vec3 violet  = vec3(0.4, 0.2, 0.7);
    vec3 magenta = vec3(0.8, 0.1, 0.6);
    vec3 red     = vec3(0.9, 0.2, 0.2);
    vec3 orange  = vec3(1.0, 0.5, 0.1);
    vec3 color = mix(violet, magenta, smoothstep(0.0, 0.33, y));
    color = mix(color, red, smoothstep(0.33, 0.66, y));
    return mix(color, orange, smoothstep(0.66, 1.0, y));
}
void main() {
    float time = timeInMilliSeconds / 1000.0;
    float y = (clamp(calculatedPosition.y, -1.0, 1.0) + 1.0) * 0.5;
    vec3 barColor;
    barColor = defaultNebulaBarColor(y);
    vec4 baseColor = mix(vec4(barColor, 1.0), vertColor, 0.35);
    float flicker = 1.0;
    if (themeNumber <= 1u) {
        flicker = 0.95 + 0.05 * sin(time * 0.5 + calculatedPosition.y * 5.0);
    } else if (themeNumber <= 3u) {
        flicker = 0.9 + 0.1 * sin(time * 2.0 + calculatedPosition.y * 15.0);
    } else {
        flicker = 0.85 + 0.15 * sin(time * 1.5 + calculatedPosition.y * 8.0);
    }
    baseColor.rgb *= flicker;
    bool insideBoundary =
        calculatedPosition.x > boundary.x &&
        calculatedPosition.x < boundary.y;
    if (insideBoundary) {
        baseColor.rgb = min(baseColor.rgb * 1.5, vec3(1.0));
    }
    Color = baseColor;
})";

    const std::string str2 = R"(#version 330 core
out vec4 Color;
void main()
{
    Color = vec4(0.3, 0.6, 1.0,0.3);
})";

const std::string str3 = R"(#version 330 core
out vec4 Color;
void main()
{
    Color = vec4(1.0, 1.0, 0.0, 0.3);
})";


    const std::string str4 = R"(#version 330 core
out vec4 Color;
void main()
{
    Color = vec4(1.0, 0.3, 0.3, 0.3);
})";

    const std::string str5 = R"(#version 330 core
out vec4 Color;
void main() {
    Color = vec4(0.3, 1.0, 0.3, 0.3);
})";


    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return str1;

        case ThemeConfig::Theme2:
            return str2;

        case ThemeConfig::Theme3:
            return str3;

        case ThemeConfig::Theme4:
            return str4;

        case ThemeConfig::Theme5:
            return str5;

        default:
            return str1;
    }
}
