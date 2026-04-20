#include "BackgroundColorSettings.hpp"


BackgroundColorSettings::BackgroundColorSettings(const std::string &value) : value(value)
{
}

std::string BackgroundColorSettings::getInfo()
{
    return std::string(
        R"()");
}

template<>
std::string BackgroundColorSettings::getBackgroundColorSettings<Mode::Analyzer>(const ThemeConfig themeConfig)
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

        case ThemeConfig::Theme8:
            return str1;

        default:
            return str2;
    }
}

template<>
std::string BackgroundColorSettings::getBackgroundColorSettings<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    const std::string str1 = R"(#version 330 core
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


    const std::string str2 = R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform uint themeNumber;
uniform float bass;
vec3 elegantGalaxy(vec2 uv, float time) {
    vec2 center = vec2(0.5);
    uv -= center;
    uv *= 0.7;
    float angleOffset = -time * 0.2;
    float cosA = cos(angleOffset);
    float sinA = sin(angleOffset);
    vec2 rotatedUV = vec2(
        uv.x * cosA - uv.y * sinA,
        uv.x * sinA + uv.y * cosA
    );
    float baseRadius = length(rotatedUV);
    float baseAngle = atan(rotatedUV.y, rotatedUV.x);
    float wave = sin(baseRadius * 12.0 - time * 1.5 + sin(baseAngle * 6.0)) * 0.15;
    float warpedRadius = baseRadius + wave;
    float warpedAngle = baseAngle + 0.1 * sin(baseRadius * 8.0 - time);
    float spiral = sin(8.0 * warpedAngle + warpedRadius * 10.0 - time * 0.2);
    float spiralMod = smoothstep(0.4, 0.0, abs(spiral)) * smoothstep(0.0, 1.2, baseRadius);
    float noiseVal = sin(warpedRadius * 20.0 - time * 0.3 + sin(warpedAngle * 8.0)) * 0.5 + 0.5;
    float galaxyShape = spiralMod * noiseVal;
    vec3 deepSpace = vec3(0.01, 0.005, 0.015);
    vec3 coreColor = vec3(0.9, 0.6, 1.0);
    vec3 armColor = vec3(0.5, 0.3, 0.8);
    vec3 color = mix(deepSpace, armColor, galaxyShape);
    color += coreColor * pow(1.0 - baseRadius, 4.0);
    color *= 0.9 + 0.1 * sin(time * 0.5);
    return color;
}

void main()
{
    float time = timeInMilliSeconds * 0.001;
    vec2 uv = calculatedPosition.xy;
    vec3 finalColor = elegantGalaxy(uv, time);

    Color = vec4(finalColor, 1.0);
})";


    const std::string str3 = R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform uint themeNumber;
uniform float bass;
vec3 auroraBarColor(float y) {
    vec3 mint    = vec3(0.25, 0.6, 0.55);
    vec3 lilac   = vec3(0.5, 0.45, 0.6);
    vec3 rose    = vec3(0.65, 0.45, 0.5);
    vec3 dusk    = vec3(0.08, 0.08, 0.1);
    vec3 color = mix(dusk, mint, smoothstep(0.0, 0.35, y));
    color = mix(color, lilac, smoothstep(0.35, 0.65, y));
    color = mix(color, rose,  smoothstep(0.65, 1.0, y));
    return color;
}
vec3 auroraEffect(vec2 uv, float time) {
    vec2 p = uv;
    float warp1 = sin(p.x * 8.0 + time * 1.5);
    float warp2 = sin(p.x * 15.0 + time * 0.9 + p.y * 3.0);
    float warp3 = sin(p.y * 10.0 + time * 1.2);
    p.y += 0.1 * warp1 + 0.05 * warp2 + 0.03 * warp3;
    float bands = sin((p.y + time * 0.4) * 30.0 + sin(p.x * 5.0 + time)) * 0.5 + 0.5;
    vec3 baseColor = auroraBarColor(clamp(p.y, 0.0, 1.0));
    float glow = pow(bands, 1.5) * 1.2;
    return clamp(baseColor * glow, 0.0, 1.0);
}
void main()
{
    float time = timeInMilliSeconds * 0.001;
    vec2 uv = calculatedPosition.xy;
    vec3 finalColor = auroraEffect(uv, time);
    Color = vec4(finalColor, 1.0);
})";

    const std::string str4 = R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform uint themeNumber;
uniform float bass;
float noise(vec2 p) {
    return sin(p.x * 5.0 + p.y * 3.0) * cos(p.y * 7.0 + p.x * 2.0);
}
vec3 nebulaEffect(vec2 uv, float time) {
    vec2 warped = uv * 2.0 + vec2(
        cos(time * 0.3 + uv.y * 6.0),
        sin(time * 0.4 + uv.x * 5.0)
    );
    float n = noise(warped + time * 0.5) * 0.5 + 0.5;
    return mix(vec3(0.05, 0.05, 0.1), vec3(0.4, 0.3, 0.5), n);
}
void main()
{
    float time = timeInMilliSeconds * 0.001;
    vec2 uv = calculatedPosition.xy;
    vec3 finalColor = nebulaEffect(uv, time);

    Color = vec4(finalColor, 1.0);
})";


    const std::string str5 = R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform uint themeNumber;
uniform float bass;
vec3 synthwaveEffect(vec2 uv, float time) {
    float wave = sin((uv.x + time * 0.5) * 15.0) * 0.15 + 0.55;
    float wave2 = sin((uv.y * 20.0 + time) * 3.0) * 0.15 + 0.55;
    float gridX = abs(fract(uv.x * 40.0) - 0.5);
    float gridY = abs(fract(uv.y * 40.0) - 0.5);
    float grid = smoothstep(0.0, 0.02, 0.05 - gridX) + smoothstep(0.0, 0.02, 0.05 - gridY);
    vec3 neonPink = vec3(0.6, 0.1, 0.5);
    vec3 neonBlue = vec3(0.0, 0.5, 0.7);
    vec3 waveColor = mix(neonPink, neonBlue, wave * wave2);
    vec3 gridColor = vec3(1.0, 0.6, 0.9) * grid * 0.4;
    vec3 background = mix(vec3(0.05, 0.0, 0.1), vec3(0.15, 0.0, 0.3), uv.y);
    return clamp(background + waveColor * 0.4 + gridColor, 0.0, 1.0);
}
void main()
{
    float time = timeInMilliSeconds * 0.001;
    vec2 uv = calculatedPosition.xy;
    vec3 finalColor = synthwaveEffect(uv, time);
    Color = vec4(finalColor, 1.0);
})";

    const std::string str6 =  R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform uint themeNumber;
uniform float bass;
vec3 synthwaveEffect(vec2 uv, float time) {
    float wave = sin((uv.x + time * 0.5) * 15.0) * 0.15 + 0.55;
    float wave2 = sin((uv.y * 20.0 + time) * 3.0) * 0.15 + 0.55;
    float gridX = abs(fract(uv.x * 40.0) - 0.5);
    float gridY = abs(fract(uv.y * 40.0) - 0.5);
    float grid = smoothstep(0.0, 0.02, 0.05 - gridX) + smoothstep(0.0, 0.02, 0.05 - gridY);
    vec3 neonPink = vec3(0.6, 0.1, 0.5);
    vec3 neonBlue = vec3(0.0, 0.5, 0.7);
    vec3 waveColor = mix(neonPink, neonBlue, wave * wave2);
    vec3 gridColor = vec3(1.0, 0.6, 0.9) * grid * 0.4;
    vec3 background = mix(vec3(0.05, 0.0, 0.1), vec3(0.15, 0.0, 0.3), uv.y);
    return clamp(background + waveColor * 0.4 + gridColor, 0.0, 1.0);
}
void main()
{
    float time = timeInMilliSeconds * 0.001;
    vec2 uv = calculatedPosition.xy;
    vec3 finalColor = synthwaveEffect(uv, time);
    Color = vec4(finalColor, 1.0);
})";

    const std::string str9 =  R"(#version 330 core
out vec4 Color;
void main()
{
    Color = vec4(0.0, 0.0, 0.0, 1);
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
        return str6;

    default:
        return str9;
    }
}

template<>
std::string BackgroundColorSettings::getBackgroundColorSettings<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
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

    const std::string str2 =  R"(
#version 330 core
out vec4 Color;

void main()
{
    Color = vec4(0.05, 0.05, 0.15, 1.0);
}
)";

    const std::string str4 =  R"(#version 330 core
out vec4 Color;
void main()
{
    Color = vec4(0.0, 0.0, 0.0, 1);
})";

    const std::string str5 =  R"(#version 330 core

in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;

uniform float timeInMilliSeconds;
uniform uint themeNumber;
uniform float bass;

vec3 auroraBarColor(float y) {
    vec3 mint  = vec3(0.25, 0.6, 0.55);
    vec3 lilac = vec3(0.5, 0.45, 0.6);
    vec3 rose  = vec3(0.65, 0.45, 0.5);
    vec3 dusk  = vec3(0.08, 0.08, 0.1);

    vec3 color = mix(dusk, mint, smoothstep(0.0, 0.35, y));
    color = mix(color, lilac, smoothstep(0.35, 0.65, y));
    color = mix(color, rose,  smoothstep(0.65, 1.0, y));

    return color;
}

vec3 auroraEffect(vec2 uv, float time) {
    vec2 p = uv;

    float warp1 = sin(p.x * 2.5 + time * 0.8);
    float warp2 = sin(p.x * 5.0 + time * 0.5 + p.y * 1.2);
    float warp3 = sin(p.y * 3.0 + time * 0.6);

    p.y += 0.15 * warp1 + 0.07 * warp2 + 0.05 * warp3;

    float bands = sin((p.y + time * 0.25) * 8.0
                      + sin(p.x * 2.0 + time * 0.5)) * 0.5 + 0.5;

    vec3 baseColor = auroraBarColor(clamp(p.y, 0.0, 1.0));

    float glow = pow(bands, 1.6) * 1.2;

    return clamp(baseColor * glow, 0.0, 1.0);
}

void main()
{
    float time = timeInMilliSeconds * 0.0005;
    vec2 uv = calculatedPosition.xy;

    vec3 finalColor = auroraEffect(uv, time);

    Color = vec4(finalColor, 1.0);
}
)";

    const std::string str6 =  R"(
#version 330 core
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

    vec4 baseColor = mix(vec4(barColor, 1.0), vertColor, 0.0);
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
}

)";


    const std::string str7 =  R"(
#version 330 core
    in vec4 calculatedPosition;
    in vec4 vertColor;
    out vec4 Color;
    uniform float timeInMilliSeconds;
    uniform uint themeNumber;
    uniform float bass;
    vec3 synthwaveEffect(vec2 uv, float time) {
        float wave = sin((uv.x + time * 0.5) * 15.0) * 0.15 + 0.55;
        float wave2 = sin((uv.y * 20.0 + time) * 3.0) * 0.15 + 0.55;
        float gridX = abs(fract(uv.x * 40.0) - 0.5);
        float gridY = abs(fract(uv.y * 40.0) - 0.5);
        float grid = smoothstep(0.0, 0.02, 0.05 - gridX) + smoothstep(0.0, 0.02, 0.05 - gridY);
        vec3 neonPink = vec3(0.6, 0.1, 0.5);
        vec3 neonBlue = vec3(0.0, 0.5, 0.7);
        vec3 waveColor = mix(neonPink, neonBlue, wave * wave2);
        vec3 gridColor = vec3(1.0, 0.6, 0.9) * grid * 0.4;
        vec3 background = mix(vec3(0.05, 0.0, 0.1), vec3(0.15, 0.0, 0.3), uv.y);
        return clamp(background + waveColor * 0.4 + gridColor, 0.0, 1.0);
    }
    void main()
    {
        float time = timeInMilliSeconds * 0.001;
        vec2 uv = calculatedPosition.xy;
        vec3 finalColor = synthwaveEffect(uv, time);
        Color = vec4(finalColor, 1.0);
    }
)";



    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return str1;

        case ThemeConfig::Theme2:
            return str2;
        case ThemeConfig::Theme3:
            return str2;
        case ThemeConfig::Theme4:
            return str4;
        case ThemeConfig::Theme5:
            return str5;
        case ThemeConfig::Theme6:
            return str6;
        case ThemeConfig::Theme7:
            return str7;
        case ThemeConfig::Theme8:
            return str1;
        case ThemeConfig::Theme9:
            return str2;
        default:
            return str1;
    }
}

BackgroundColorSettings::BackgroundColorSettings(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
        case Mode::Analyzer:
            value = getBackgroundColorSettings<Mode::Analyzer>(themeConfig);
            break;
        case Mode::Visualizer:
            value = getBackgroundColorSettings<Mode::Visualizer>(themeConfig);
            break;
        case Mode::StereoRmsMeter:
            value = getBackgroundColorSettings<Mode::StereoRmsMeter>(themeConfig);
            break;
    }
}
