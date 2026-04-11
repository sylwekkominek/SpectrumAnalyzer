#include "AdvancedColorSettings.hpp"


AdvancedColorSettings::AdvancedColorSettings(const std::string &value) : value(value)
{
}

std::string AdvancedColorSettings::getInfo()
{
    return std::string(
        R"(//Description: The application first determines how many frequency bins are defined by the user, based on the number of frequencies provided by the getFrequencies() function. Then, it divides the screen into the same number of vertical bars — one for each frequency. Each bar is built using two triangles (6 vertices), which form a rectangle defined by 4 unique corner points in 2D space (x, y). These bars are then sent to the GPU with their initial positions. At this stage, all bars are evenly spaced and aligned. As the application analyzes the audio (or other signal) in real time, it calculates the power of each frequency. Based on this power, the height or vertical position of each bar is updated dynamically to reflect the current signal strength. The vertex shader updates the positions of the bar's corners depending on the signal’s power for that frequency. The GPU then fills in all the pixels between those corners by generating fragments — and each of these fragments is processed by the fragment shader. One of the values the fragment shader receives is calculatedPosition, which (in simplified terms) tells it how high up the current pixel is within its rectangle. This vertical position is used to control how the pixel will be colored. In addition, the shader receives vertColor values — these are colors assigned to each corner of the rectangle using application-side functions like getColorsOfRectangle() and getColorsOfDynamicMaxHoldRectangle(). The GPU automatically blends these colors across the surface of the rectangle, so each pixel gets its own color depending on where it lies. Inside the fragment shader, this blended color is combined with a vertical color gradient that transitions from blue → cyan → green → yellow → red, depending on the pixel’s height within the bar. This results in vibrant and informative color transitions that follow the signal in real time. Users can adjust the rectangle corner colors directly in the application to experiment with color effects. And because the shader computes colors for each pixel independently, there’s a lot of flexibility for customization. More advanced users can even edit the shader code to fully control how colors behave. All of this runs on the GPU, which executes the fragment shader for every pixel of every bar in parallel. This makes the visualization extremely fast and smooth — even when rendering many bars at once. This method not only results in dynamic and responsive visualizations, but also allows users to experiment and learn how shaders and GPU parallelism work in real-time graphics.
//if you get errors after modification please find following log with error msg: "VS log:"
)");
}

template<>
std::string AdvancedColorSettings::getAdvancedColorSettings<Mode::Analyzer>(const ThemeConfig themeConfig)
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

        case ThemeConfig::Theme6:
            return str1;

        case ThemeConfig::Theme7:
            return str1;

        case ThemeConfig::Theme8:
            return str1;

        case ThemeConfig::Theme9:
            return str2;

        case ThemeConfig::Theme10:
            return str3;

        default:
            return str1;
    }
}

template<>
std::string AdvancedColorSettings::getAdvancedColorSettings<Mode::Visualizer>(const ThemeConfig themeConfig)
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

    const std::string str2 = R"()";

    const std::string str3 = R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform vec2 boundary;
uniform uint themeNumber;
vec3 auroraBarColor(float y) {
    vec3 teal   = vec3(0.0, 0.8, 0.7);
    vec3 green  = vec3(0.2, 1.0, 0.5);
    vec3 purple = vec3(0.4, 0.2, 0.6);
    vec3 pink   = vec3(0.9, 0.4, 0.7);
    vec3 color = mix(teal, green, smoothstep(0.0, 0.3, y));
    color = mix(color, purple, smoothstep(0.3, 0.7, y));
    return mix(color, pink, smoothstep(0.7, 1.0, y));
}
void main() {
    float time = timeInMilliSeconds / 1000.0;
    float y = (clamp(calculatedPosition.y, -1.0, 1.0) + 1.0) * 0.5;
    vec3 barColor = auroraBarColor(y);
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

    const std::string str4 = R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform vec2 boundary;
uniform uint themeNumber;

vec3 synthwaveBarColor(float y) {
    vec3 darkPink     = vec3(0.9, 0.1, 0.5);
    vec3 electricBlue = vec3(0.0, 0.7, 1.0);
    vec3 neonPurple   = vec3(0.7, 0.0, 0.9);
    vec3 color = mix(darkPink, electricBlue, smoothstep(0.0, 0.5, y));
    return mix(color, neonPurple, smoothstep(0.5, 1.0, y));
}
void main() {
    float time = timeInMilliSeconds / 1000.0;
    float y = (clamp(calculatedPosition.y, -1.0, 1.0) + 1.0) * 0.5;
    vec3 barColor = synthwaveBarColor(y);
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

    const std::string str6 = R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform vec2 boundary;
uniform uint themeNumber;
float hash(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}
float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x)
         + (c - a) * u.y * (1.0 - u.x)
         + (d - b) * u.x * u.y;
}
float fbm(vec2 p) {
    float total = 0.0;
    float amp = 0.5;
    for (int i = 0; i < 5; i++) {
        total += amp * noise(p);
        p *= 2.0;
        amp *= 0.5;
    }
    return total;
}
vec3 smokeBarColor() {
    float time = timeInMilliSeconds / 1000.0;
    vec2 uv = calculatedPosition.xy * 0.5 + 0.5;
    vec2 motion = vec2(
        sin(time * 0.6) * 0.35,
        -time * 0.35
    );
    vec2 p = uv * vec2(3.0, 5.0) + motion;
    float s = fbm(p);
    s = pow(s, 1.6);
    float fade = smoothstep(0.0, 0.9, 1.0 - uv.y);
    s *= fade;
    return mix(vec3(0.15), vec3(0.95), s);
}
void main() {
    float time = timeInMilliSeconds / 1000.0;
    float y = (clamp(calculatedPosition.y, -1.0, 1.0) + 1.0) * 0.5;
    vec3 barColor= smokeBarColor();

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

    const std::string str7 = R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform vec2 boundary;
uniform uint themeNumber;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
vec3 rainbowBarColor() {
    float time = timeInMilliSeconds * 0.001;
    float x = (calculatedPosition.x + 1.0) * 0.5;
    float y = (calculatedPosition.y + 1.0) * 0.5;
    float hue = mod(time * 0.05 + x * 0.5 + y * 0.5, 1.0);
    float saturation = 0.8 + 0.2 * sin(time * 1.0 + x * 10.0);
    float value = 0.7 + 0.3 * sin(time * 2.0 + y * 15.0);
    vec3 color = hsv2rgb(vec3(hue, saturation, value));
    float wave = sin(10.0 * (x + y) - time * 2.0);
    float brightness = 0.5 + 0.5 * smoothstep(0.0, 1.0, wave);
    color *= brightness;
    float flicker = 0.8 + 0.2 * sin(time * 10.0 + (x + y) * 20.0);
    color *= flicker;
    return color;
}
void main() {
    float time = timeInMilliSeconds / 1000.0;
    float y = (clamp(calculatedPosition.y, -1.0, 1.0) + 1.0) * 0.5;
    vec3 barColor = rainbowBarColor();

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


    const std::string str8 = R"(#version 330 core

     in vec4 calculatedPosition;
     in vec4 vertColor;
     out vec4 Color;

     void main()
     {
         vec3 tmpColor;

         float t = clamp(calculatedPosition.y, -1.0, 1.0);
         float y = (t + 1.0) * 0.5;

         vec3 green    = vec3(0.0, 1.0, 0.0);
         vec3 yellow   = vec3(1.0, 1.0, 0.0);
         vec3 orange   = vec3(1.0, 0.5, 0.0);
         vec3 red      = vec3(1.0, 0.0, 0.0);

         tmpColor = mix(green, yellow, smoothstep(0.0, 0.5, y));
         tmpColor = mix(tmpColor, orange, smoothstep(0.5, 0.75, y));
         tmpColor = mix(tmpColor, red, smoothstep(0.75, 1.0, y));

         Color = mix(vec4(tmpColor, 1.0), vertColor, 0.4);
     })";

    const std::string str9 = R"(#version 330 core

in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;

void main()
{
    vec3 tmpColor;

    float t = clamp(calculatedPosition.y, -1.0, 1.0);
    float y = (t + 1.0) * 0.5;

    vec3 darkBlue  = vec3(0.0, 0.0, 0.5);
    vec3 lightBlue = vec3(0.2, 0.6, 1.0);
    vec3 green     = vec3(0.0, 1.0, 0.0);
    vec3 yellow    = vec3(1.0, 1.0, 0.0);
    vec3 red       = vec3(1.0, 0.0, 0.0);

    tmpColor = mix(darkBlue,  lightBlue, smoothstep(0.0, 0.25, y));
    tmpColor = mix(tmpColor,  green,     smoothstep(0.25, 0.5, y));
    tmpColor = mix(tmpColor,  yellow,    smoothstep(0.5, 0.75, y));
    tmpColor = mix(tmpColor,  red,       smoothstep(0.75, 1.0, y));

    Color = mix(vec4(tmpColor, 1.0), vertColor, 0.4);
})";

    const std::string str10 = R"(#version 330 core

in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;

vec3 hsv2rgb(vec3 c)
{
    vec3 rgb = clamp(abs(mod(c.x*6.0 + vec3(0.0,4.0,2.0),6.0)-3.0)-1.0,0.0,1.0);
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

void main()
{
    float t = clamp(calculatedPosition.x, -1.0, 1.0);
    float x = (t + 1.0) * 0.5;

    vec3 rainbow = hsv2rgb(vec3(x, 1.0, 1.0));

    Color = mix(vec4(rainbow, 1.0), vertColor, 0.4);
})";

    switch(themeConfig)
    {
    case ThemeConfig::Theme1:
        return str1;

    case ThemeConfig::Theme2:
        return str1;

    case ThemeConfig::Theme3:
        return str3;

    case ThemeConfig::Theme4:
        return str4;

    case ThemeConfig::Theme5:
        return str4;

    case ThemeConfig::Theme6:
        return str6;

    case ThemeConfig::Theme7:
        return str7;

    case ThemeConfig::Theme8:
        return str8;

    case ThemeConfig::Theme9:
        return str9;

    case ThemeConfig::Theme10:
        return str10;

    default:
        return str1;
    }
}


template<>
std::string AdvancedColorSettings::getAdvancedColorSettings<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
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

in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;

uniform float bass;
uniform float spectrum[64];

vec3 winampBars(vec2 uv)
{
    float bars = 64.0;
    float spacing = 0.25;

    float xNorm = (uv.x + 1.0) * 0.5;
    float barIndexF = xNorm * bars;
    int barIndex = int(floor(barIndexF));
    float localX = fract(barIndexF);

    barIndex = clamp(barIndex, 0, 63);

    if(localX < spacing || localX > 1.0 - spacing)
        return vec3(0.0);

    float height = spectrum[barIndex];
    height *= (0.4 + bass * 2.0);
    height = clamp(height, 0.0, 1.0);

    float t = clamp(uv.y, -1.0, 1.0);
    float yNorm = (t + 1.0) * 0.5;

    float segments = 60.0;
    float segY = fract(yNorm * segments);
    if(segY < 0.25)
        return vec3(0.0);

    vec3 green  = vec3(0.0, 1.0, 0.0);
    vec3 yellow = vec3(1.0, 1.0, 0.0);
    vec3 orange = vec3(1.0, 0.5, 0.0);
    vec3 red    = vec3(1.0, 0.0, 0.0);

    vec3 color;
    if(yNorm < 0.5)
        color = mix(green, yellow, yNorm * 2.0);
    else if(yNorm < 0.75)
        color = mix(yellow, orange, (yNorm - 0.5) * 4.0);
    else
        color = mix(orange, red, (yNorm - 0.75) * 4.0);

    return color;
}

void main()
{
    vec2 uv = calculatedPosition.xy;

    vec3 finalColor = winampBars(uv);

    Color = vec4(finalColor, 1.0);
})";

    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return str1;

        case ThemeConfig::Theme2:
            return str2;
        default:
            return str1;
    }
}

AdvancedColorSettings::AdvancedColorSettings(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
        case Mode::Analyzer:
            value = getAdvancedColorSettings<Mode::Analyzer>(themeConfig);
            break;
        case Mode::Visualizer:
            value = getAdvancedColorSettings<Mode::Visualizer>(themeConfig);
            break;
        case Mode::StereoRmsMeter:
            value = getAdvancedColorSettings<Mode::StereoRmsMeter>(themeConfig);
            break;
    }
}
