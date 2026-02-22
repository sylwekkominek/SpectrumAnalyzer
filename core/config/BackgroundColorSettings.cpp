#include "BackgroundColorSettings.hpp"


BackgroundColorSettings::BackgroundColorSettings(const std::string &value) : value(value)
{
}

BackgroundColorSettings::BackgroundColorSettings(const ThemeConfig themeConfig) : value(getBackgroundColorSettings(themeConfig))
{
}

std::string BackgroundColorSettings::getInfo()
{
    return std::string(
        R"()");
}

std::string BackgroundColorSettings::getBackgroundColorSettings(const ThemeConfig themeConfig)
{
    const std::string str1 =  R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform uint themeNumber;
uniform float bass;
float noiseSimple(vec2 p) {
    return sin(p.x * 3.0) * cos(p.y * 2.0);
}
vec3 nebulaGlowEffect(vec2 uv, float time, float bass) {
    vec2 warped = uv * 2.0 + vec2(
        cos(time * 0.3 + uv.y * 6.0),
        sin(time * 0.4 + uv.x * 5.0)
    );
    float pattern = noiseSimple(warped + time * 0.5) * 0.5 + 0.5;
    float pulse = 0.6 + 0.3 * sin(time * 0.7 + length(uv) * 2.0) * bass;
    pattern *= pulse;
    vec3 glow = mix(vec3(0.05, 0.05, 0.1), vec3(0.3, 0.2, 0.5), smoothstep(0.0, 1.0, pattern));
    float y = (clamp(uv.y, -1.0, 1.0) + 1.0) * 0.5;
    vec3 gradient = mix(vec3(0.01, 0.01, 0.02), vec3(0.1, 0.1, 0.2), y);
    return mix(gradient, glow, 0.6);
}
void main()
{
    float time = timeInMilliSeconds * 0.001;
    vec2 uv = calculatedPosition.xy;
    vec3 finalColor = nebulaGlowEffect(uv, time, bass);
    Color = vec4(finalColor, 1.0);
})";

    const std::string str2 =  R"(#version 330 core
out vec4 Color;
void main()
{
    Color = vec4(0.1, 0.1, 0.1, 0.5);
})";

    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return str1;

        default:
            return str2;
    }
}
