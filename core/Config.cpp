/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Config.hpp"
#include <numeric>
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

SignalWindow::SignalWindow(uint16_t numberOfSamples) : value(getSignalWindow(numberOfSamples))
{
}

SignalWindow::SignalWindow(const std::vector<float> &window) : value(window)
{
}

std::vector<float> SignalWindow::getSignalWindow(uint16_t numberOfSamples)
{
    std::vector<float> window(numberOfSamples);

    for (size_t n = 0; n < numberOfSamples; ++n)
    {
        window[n] = 0.5 * (1.0 - std::cos(2.0 * PI * n / (numberOfSamples - 1)));
    }

    return window;
}

ScalingFactor::ScalingFactor(const float value) : value(value)
{
}

ScalingFactor::ScalingFactor(const std::vector<float> &window) : value(getScalingFactor(window))
{
}

float ScalingFactor::getScalingFactor(const std::vector<float> &window)
{
    const float windowSum = std::accumulate(window.begin(), window.end(), 0.0);
    const float windowAverage = windowSum / window.size();
    const float amplitudeCorrection = 1.0 / windowAverage;
    return amplitudeCorrection;
}

AdvancedColorSettings::AdvancedColorSettings():
    value(getAdvancedColorSettings())
{
}

AdvancedColorSettings::AdvancedColorSettings(const std::string &shader) : value(shader)
{
}

std::string AdvancedColorSettings::getAdvancedColorSettings()
{
        return R"(#version 330 core
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
vec3 nebulaBarColor(float y) {
    vec3 violet  = vec3(0.4, 0.2, 0.7);
    vec3 magenta = vec3(0.8, 0.1, 0.6);
    vec3 red     = vec3(0.9, 0.2, 0.2);
    vec3 orange  = vec3(1.0, 0.5, 0.1);
    vec3 color = mix(violet, magenta, smoothstep(0.0, 0.33, y));
    color = mix(color, red, smoothstep(0.33, 0.66, y));
    return mix(color, orange, smoothstep(0.66, 1.0, y));
}
vec3 auroraBarColor(float y) {
    vec3 teal   = vec3(0.0, 0.8, 0.7);
    vec3 green  = vec3(0.2, 1.0, 0.5);
    vec3 purple = vec3(0.4, 0.2, 0.6);
    vec3 pink   = vec3(0.9, 0.4, 0.7);
    vec3 color = mix(teal, green, smoothstep(0.0, 0.3, y));
    color = mix(color, purple, smoothstep(0.3, 0.7, y));
    return mix(color, pink, smoothstep(0.7, 1.0, y));
}
vec3 synthwaveBarColor(float y) {
    vec3 darkPink     = vec3(0.9, 0.1, 0.5);
    vec3 electricBlue = vec3(0.0, 0.7, 1.0);
    vec3 neonPurple   = vec3(0.7, 0.0, 0.9);
    vec3 color = mix(darkPink, electricBlue, smoothstep(0.0, 0.5, y));
    return mix(color, neonPurple, smoothstep(0.5, 1.0, y));
}
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
vec3 firePalette(float t) {
    vec3 c1 = vec3(0.6, 0.1, 0.0);
    vec3 c2 = vec3(1.0, 0.4, 0.0);
    vec3 c3 = vec3(1.0, 1.0, 0.3);
    if (t < 0.5)
        return mix(c1, c2, t * 2.0);
    else
        return mix(c2, c3, (t - 0.5) * 2.0);
}
vec3 fireBarColor() {
    float time = timeInMilliSeconds / 1000.0;
    vec2 uv = calculatedPosition.xy * 0.5 + 0.5;
    float yOffset = time * 0.7;
    float noisePos = uv.y * 3.0 - yOffset;
    float n = fbm(vec2(uv.x * 3.0, noisePos));
    n += 0.5 * fbm(vec2(uv.x * 6.0, noisePos * 2.0));
    n += 0.25 * fbm(vec2(uv.x * 12.0, noisePos * 4.0));
    n = clamp(n, 0.0, 1.0);
    float shape = smoothstep(0.0, 0.7, uv.y);
    float intensity = n * (1.0 - shape);
    vec3 fireCol = firePalette(intensity);
    fireCol *= smoothstep(0.0, 0.2, 1.0 - uv.y);
    return fireCol;
}
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
float fakeNoise(vec2 p) {
    return sin(p.x) * sin(p.y);
}
vec3 nebulaGlowColor(float x) {
    return mix(vec3(0.2, 0.0, 0.4),
               vec3(1.0, 0.3, 0.8),
               smoothstep(0.0, 1.0, x));
}
vec3 nebulaEnergyBarColor() {
    float time = timeInMilliSeconds / 1000.0;
    vec2 uv = calculatedPosition.xy;
    vec2 warped = uv * 3.0 + vec2(
        sin(time * 0.5 + uv.y * 5.0),
        cos(time * 0.4 + uv.x * 5.0)
    );
    float pattern = fakeNoise(warped + time) * 0.5 + 0.5;
    float pulse = 0.5 + 0.5 * sin(time * 2.0 + length(uv) * 5.0);
    pattern *= pulse;
    vec3 glow = nebulaGlowColor(pattern);
    float y = (clamp(calculatedPosition.y, -1.0, 1.0) + 1.0) * 0.5;
    vec3 baseGradient = mix(vec3(0.0, 0.0, 0.1),
                            vec3(0.4, 0.0, 0.2),
                            y);
    return mix(baseGradient, glow, 0.7);
}
void main() {
    float time = timeInMilliSeconds / 1000.0;
    float y = (clamp(calculatedPosition.y, -1.0, 1.0) + 1.0) * 0.5;
    vec3 barColor;
    if (themeNumber == 0u) {
        barColor = defaultNebulaBarColor(y);
    } else if (themeNumber == 1u) {
        barColor = nebulaBarColor(y);
    } else if (themeNumber == 2u) {
        barColor = nebulaBarColor(y);
    } else if (themeNumber == 3u) {
        barColor = auroraBarColor(y);
    } else if ((themeNumber >= 4u) && (themeNumber <= 5u)) {
        barColor = synthwaveBarColor(y);
    } else if (themeNumber == 6u) {
        barColor = smokeBarColor();
    } else if (themeNumber == 7u) {
        barColor = fireBarColor();
    } else if (themeNumber == 8u) {
        barColor = rainbowBarColor();
    } else if (themeNumber == 9u) {
        barColor = nebulaEnergyBarColor();
    } else {
        barColor = defaultNebulaBarColor(y);
    }
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
}

BackgroundColorSettings::BackgroundColorSettings():
        value(getBackgroundColorSettings())
{
}

BackgroundColorSettings::BackgroundColorSettings(const std::string &shader) : value(shader)
{
}

std::string BackgroundColorSettings::getBackgroundColorSettings()
{
        return R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform uint themeNumber;
uniform float bass;
float noise(vec2 p) {
    return sin(p.x * 5.0 + p.y * 3.0) * cos(p.y * 7.0 + p.x * 2.0);
}
float noiseSimple(vec2 p) {
    return sin(p.x * 3.0) * cos(p.y * 2.0);
}
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
vec3 nebulaEffect(vec2 uv, float time) {
    vec2 warped = uv * 2.0 + vec2(
        cos(time * 0.3 + uv.y * 6.0),
        sin(time * 0.4 + uv.x * 5.0)
    );
    float n = noise(warped + time * 0.5) * 0.5 + 0.5;
    return mix(vec3(0.05, 0.05, 0.1), vec3(0.4, 0.3, 0.5), n);
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
    vec3 finalColor;
    switch (themeNumber)
    {
        case 1u:
            finalColor = nebulaGlowEffect(uv, time, bass);
            break;
        case 2u:
            finalColor = elegantGalaxy(uv, time);
            break;
        case 3u:
            finalColor = auroraEffect(uv, time);
            break;
        case 4u:
            finalColor = nebulaEffect(uv, time);
            break;
        case 5u:
            finalColor = synthwaveEffect(uv, time);
            break;
        case 6u:
        case 7u:
        case 8u:
        case 9u:
            finalColor = vec3(0.0);
            break;
        default:
            finalColor = nebulaGlowEffect(uv, time, bass);
            break;
    }
    Color = vec4(finalColor, 1.0);
})";
}

std::ostream& operator<<(std::ostream& os, const Freqs &freqs)
{
    os <<"frequencies: ";

    for(const auto &el: freqs.value)
    {
        os <<el<<" ";
    }
    os <<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const MaxQueueSize &maxQueueSize)
{
    os <<"maxQueueSize: "<<maxQueueSize.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const PythonDataSourceEnabled &pythonDataSourceEnabled)
{
    os <<"pythonDataSourceEnabled: "<<pythonDataSourceEnabled.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DefaultFullscreenState& defaultFullscreenState)
{
    os <<"defaultFullscreenState: "<<defaultFullscreenState.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const NumberOfRectangles &numberOfRectangles)
{
    os <<"numberOfRectangles: "<<numberOfRectangles.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const GapWidthInRelationToRectangleWidth& gapWidthInRelationToRectangleWidth)
{
    os <<"gapWidthInRelationToRectangleWidth: "<<gapWidthInRelationToRectangleWidth.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const MaximizedWindowSize &maximizedWindowSize)
{
    os <<"maximizedWindowSize: "<<maximizedWindowSize.value.first<<" "<<maximizedWindowSize.value.second<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const NormalWindowSize &normalWindowSize)
{
    os <<"maximizedWindowSize: "<<normalWindowSize.value.first<<" "<<normalWindowSize.value.second<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const NumberOfSamples &numberOfSamples)
{
    os <<"numberOfSamples: "<<numberOfSamples.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const SamplingRate &samplingRate)
{
    os <<"samplingRate: "<<samplingRate.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DesiredFrameRate &desiredFrameRate)
{
    os <<"desiredFrameRate: "<<desiredFrameRate.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const NumberOfSignalsForAveraging &numberOfSignalsForAveraging)
{
    os <<"numberOfSignalsForAveraging: "<<numberOfSignalsForAveraging.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const NumberOfSignalsForMaxHold &numberOfSignalsForMaxHold)
{
    os <<"numberOfSignalsForMaxHold: "<<numberOfSignalsForMaxHold.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const AlphaFactor &alphaFactor)
{
    os <<"alphaFactor: "<<alphaFactor.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const ScalingFactor &scalingFactor)
{
    os <<"scalingFactor: "<<scalingFactor.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const OffsetFactor &offsetFactor)
{
    os <<"offsetFactor: "<<offsetFactor.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const RectanglesVisibilityState &rectanglesVisibilityState)
{
    os <<"rectanglesVisibilityState: "<<rectanglesVisibilityState.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const LinesVisibilityState &linesVisibilityState)
{
    os <<"linesVisibilityState: "<<linesVisibilityState.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldVisibilityState &dynamicMaxHoldVisibilityState)
{
    os <<"dynamicMaxHoldVisibilityState: "<<dynamicMaxHoldVisibilityState.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldSecondaryVisibilityState &dynamicMaxHoldSecondaryVisibilityState)
{
    os <<"dynamicMaxHoldSecondaryVisibilityState: "<<dynamicMaxHoldSecondaryVisibilityState.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldRectangleHeightInPercentOfScreenSize &dynamicMaxHoldRectangleHeightInPercentOfScreenSize)
{
    os <<"dynamicMaxHoldRectangleHeightInPercentOfScreenSize: "<<dynamicMaxHoldRectangleHeightInPercentOfScreenSize.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldSpeedOfFalling &dynamicMaxHoldSpeedOfFalling)
{
    os <<"dynamicMaxHoldSpeedOfFalling: "<<dynamicMaxHoldSpeedOfFalling.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldAccelerationStateOfFalling & dynamicMaxHoldAccelerationStateOfFalling)
{
    os <<"dynamicMaxHoldAccelerationStateOfFalling: "<<dynamicMaxHoldAccelerationStateOfFalling.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const HorizontalLinePositions &horizontalLinePositions)
{
    os <<"horizontalLinePositions: ";

    for(const auto &el: horizontalLinePositions.value)
    {
        os <<el<<" ";
    }
    os <<std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const VerticalLinePositions &verticalLinePositions)
{
    os <<"verticalLinePositions: ";

    for(const auto &el: verticalLinePositions.value)
    {
        os <<el<<" ";
    }
    os <<std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const FrequencyTextPositions &frequencyTextPositions)
{
    os <<"frequencyTextPositions: ";

    for(const auto &el: frequencyTextPositions.value)
    {
        os <<el<<" ";
    }
    os <<std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const ColorOfStaticText &colorOfStaticText)
{
    os<<"colorOfStaticText: ";
    for(auto & component: colorOfStaticText.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const ColorOfLine &colorOfLine)
{
    os<<"colorOfLine: ";
    for(auto & component: colorOfLine.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const ColorOfStaticLines &colorOfStaticLines)
{
    os<<"colorOfStaticLines: ";
    for(auto & component: colorOfStaticLines.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const ColorOfDynamicMaxHoldLine &colorOfDynamicMaxHoldLine)
{
    os<<"colorOfDynamicMaxHoldLine: ";
    for(auto & component: colorOfDynamicMaxHoldLine.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const ColorOfDynamicMaxHoldSecondaryLine &colorOfDynamicMaxHoldSecondaryLine)
{
    os<<"colorOfDynamicMaxHoldSecondaryLine: ";
    for(auto & component: colorOfDynamicMaxHoldSecondaryLine.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
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

std::ostream& operator<<(std::ostream& os, const SignalWindow& signalWindow)
{
    os <<"signalWindow size: "<<signalWindow.value.size()<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Configuration &config)
{
    os<<config.data.get<PythonDataSourceEnabled>();
    os<<config.data.get<DefaultFullscreenState>();
    os<<config.data.get<MaximizedWindowSize>();
    os<<config.data.get<NormalWindowSize>();
    os<<config.data.get<NumberOfRectangles>();
    os<<config.data.get<GapWidthInRelationToRectangleWidth>();
    os<<config.data.get<NumberOfSamples>();
    os<<config.data.get<SamplingRate>();
    os<<config.data.get<DesiredFrameRate>();
    os<<config.data.get<NumberOfSignalsForAveraging>();
    os<<config.data.get<NumberOfSignalsForMaxHold>();
    os<<config.data.get<AlphaFactor>();
    os<<config.data.get<MaxQueueSize>();
    os<<config.data.get<ScalingFactor>();
    os<<config.data.get<OffsetFactor>();
    os<<config.data.get<RectanglesVisibilityState>();
    os<<config.data.get<LinesVisibilityState>();
    os<<config.data.get<DynamicMaxHoldVisibilityState>();
    os<<config.data.get<DynamicMaxHoldSecondaryVisibilityState>();
    os<<config.data.get<DynamicMaxHoldRectangleHeightInPercentOfScreenSize>();
    os<<config.data.get<DynamicMaxHoldAccelerationStateOfFalling>();
    os<<config.data.get<Freqs>();
    os<<config.data.get<SignalWindow>();
    os<<config.data.get<HorizontalLinePositions>();
    os<<config.data.get<VerticalLinePositions>();
    os<<config.data.get<ColorOfLine>();
    os<<config.data.get<ColorOfStaticText>();
    os<<config.data.get<ColorOfStaticLines>();
    os<<config.data.get<ColorOfDynamicMaxHoldLine>();
    os<<config.data.get<ColorOfDynamicMaxHoldSecondaryLine>();
    os<<config.data.get<ColorsOfRectangle>();
    os<<config.data.get<ColorsOfDynamicMaxHoldRectangle>();

    return os;
}

std::string Freqs::getInfo()
{
    return std::string(
R"(//Description: This function returns a list of target frequencies (in Hz) that the application will attempt to visualize using vertical bars on the screen. However, it’s important to understand that the actual signal analysis is carried out using an FFT (Fast Fourier Transform), whose frequency resolution is determined by two key parameters: the number of samples (getNumberOfSamples()) and the sampling rate (getSamplingRate()). For example, if you use 4096 samples with a sampling rate of 44100 Hz, the frequency resolution of the FFT is approximately 10.77 Hz (calculated by dividing the sampling rate by the number of samples, i.e., 44100 / 4096). This means that the FFT output only includes energy values at specific, evenly spaced frequency steps — also known as FFT bins — such as 0 Hz, 10.77 Hz, 21.55 Hz, 32.32 Hz, 43.09 Hz, and so on. When you define a list of frequencies using getFrequencies(), the application maps each requested frequency to the nearest available FFT bin. As a result, if multiple frequencies are too close together (for example, 40 Hz, 41 Hz, and 43 Hz), they may all get mapped to the same bin — such as 43.09 Hz — and thus produce identical or nearly identical visual bars on the screen. The number of bars shown in the visualization is equal to the number of frequencies returned by this function, but this does not improve the frequency resolution of the signal analysis. The FFT is always computed over the full spectrum based on the sample size and sampling rate. Increasing the number of requested frequencies only determines how many bars are drawn and which FFT bins are sampled — not how precisely the signal is analyzed.
//Default value: (20 40 60 ... When analyzing sound, the function usually returns a list of frequencies where low frequencies are represented by closely spaced values to better distinguish subtle differences, while higher frequencies are spaced farther apart since fine details are harder to perceive. However, this approach is common but not mandatory — the frequency spacing can be adjusted based on specific needs or preferences.)
)");
}

std::string SignalWindow::getInfo()
{
    return std::string(
R"(//Description: Users can apply a custom window function to the signal before frequency analysis, which allows for experimentation with different windowing techniques and their impact on the result. This offers flexibility in shaping the signal to reduce artifacts like spectral leakage. For example, you can use window functions from the scipy.signal module—such as Hamming, Blackman, or Kaiser—to tailor the analysis to your specific needs.
//Default value: Hanning window
)");
}

std::string PythonDataSourceEnabled::getInfo()
{
    return std::string(
        R"()");
}

std::string DefaultFullscreenState::getInfo()
{
    return std::string(
R"(//Description: Determines whether the SpectrumAnalyzer application starts in fullscreen mode by default. Users can choose this to enable fullscreen startup automatically. This is especially useful on devices like Raspberry Pi, where fullscreen improves user experience and system integration.
//Default value: False (starts in windowed mode)
)");
}

std::string NumberOfRectangles::getInfo()
{
    return std::string();
}

std::string GapWidthInRelationToRectangleWidth::getInfo()
{
    return std::string();
}

std::string MaximizedWindowSize::getInfo()
{
    return std::string(
R"(//Description: The user can select the fullscreen resolution. This is particularly useful on devices like the Raspberry Pi, where the maximum resolution may cause performance issues. Allowing users to lower the resolution can improve application responsiveness and ensure smoother performance.
//Default value: 1920×1080
)");
}

std::string NormalWindowSize::getInfo()
{
    return std::string(
R"(//Description: The initial window size when the application starts in windowed mode. This setting ensures that the strips always look the way the user wants, providing a consistent visual layout without the need for manual resizing after launch.
//Default value: 1280×512
)");
}

std::string NumberOfSamples::getInfo()
{
    return std::string(
R"(//Description: Users can adjust the number of audio samples processed each time to find the best setup for them. Using fewer than 4096 samples makes the display smoother and more responsive but lowers the detail at low frequencies. For example, with fewer samples, it becomes harder to distinguish between very close frequencies like 20 Hz and 30 Hz. Increasing the number of samples improves frequency detail but can make the program less responsive.
//Default value: 4096
)");
}

std::string SamplingRate::getInfo()
{
    return std::string(
R"(//Description: This parameter is rarely changed but can be useful when analyzing audio at non-standard sample rates. It also allows experimenting with oversampling or downsampling (which PyAudio supports). Additionally, you might need to adjust it when connecting external devices with different sampling frequencies.
//Default value: 48000 Hz
)");
}

std::string DesiredFrameRate::getInfo()
{
    return std::string(
R"(//Description: Sets the target number of frames per second (FPS) displayed on the screen. Since the number of samples and sampling rate limit how frequently new data can be processed, the program increases the frame rate by using overlapping. For example, with 4096 samples at a 44100 Hz sampling rate, the default frame rate is about 11 FPS. To reach higher rates like 60 FPS, the program applies Welch’s method, processing overlapping segments of the signal to effectively analyze more data per second. This parameter also allows adjusting the application to run smoothly on hardware with limited performance, such as a Raspberry Pi, which may struggle with higher frame rates, especially at higher resolutions.
//Default value: 55 FPS
)");
}

std::string NumberOfSignalsForAveraging::getInfo()
{
    return std::string(
R"(//Description: This parameter controls how many signal samples are averaged together to produce the displayed output. Increasing the number of signals for averaging results in smoother, more stable visualizations by reducing short-term fluctuations. However, it also introduces a slight delay in responsiveness. Lower values make the output more immediate and reactive but can appear more “jumpy.”
//Default value: 1
)");
}

std::string NumberOfSignalsForMaxHold::getInfo()
{
    return std::string(
R"(//Description: Defines how many of the recent signal frames are considered when calculating the maximum hold value. This affects how quickly the displayed peak values update and start to drop, making the peak indicators appear either more stable or more dynamic.
//Default value: 5
)");
}

std::string AlphaFactor::getInfo()
{
    return std::string(
R"(//Description: Controls the amount of smoothing applied to the signal visualization. A lower alpha factor results in a smoother and more stable display, but with slower response to rapid changes. A higher alpha makes the visualization more responsive and sensitive to signal variations, although it may result in a more flickery or unstable display.
//Default value: 0.25
)");
}

std::string MaxQueueSize::getInfo()
{
    return std::string(
        R"()");
}

std::string ScalingFactor::getInfo()
{
    return std::string(
R"(//Description: Adjusts the amplitude of the signal after windowing to ensure the signal level remains accurate, since different window functions can alter the overall amplitude.
//Default value: Calculates and returns the amplitude correction based on the average value of the window.
)");
}

std::string OffsetFactor::getInfo()
{
    return std::string(
R"(//Description: Allows precise adjustment of the signal amplitude, useful when working with external devices or simply to raise the signal level if the displayed bars on the screen appear too low.
//Default value: 0
)");
}

std::string RectanglesVisibilityState::getInfo()
{
    return std::string(
        R"(//Description: Enables the visibility of rectangles on the display.
//Default value: true
)");
}

std::string LinesVisibilityState::getInfo()
{
    return std::string(
        R"(//Description: Enables the visibility of lines on the display.
//Default value: false
)");
}


std::string DynamicMaxHoldVisibilityState::getInfo()
{
    return std::string(
R"(//Description: Toggles the visibility of on-screen elements that display the peak (max hold) values over time. When enabled, markers show the highest recent signal levels.
//Default value: True
)");
}

std::string DynamicMaxHoldSecondaryVisibilityState::getInfo()
{
    return std::string(
        R"(//Description: Toggles the visibility of secondary on-screen elements that display the peak (max hold) values over time. When enabled, secondary markers show the highest recent signal levels.
//Default value: True
)");
}

std::string DynamicMaxHoldRectangleHeightInPercentOfScreenSize::getInfo()
{
    return std::string(
R"(//Description: Allows the user to control the height of the rectangles that represent dynamic max hold values. The value is given as a percentage of the screen height — for example, 1 means the rectangle will be 1% of the total screen height.
//Default value: 0.8
)");
}

std::string DynamicMaxHoldSpeedOfFalling::getInfo()
{
    return std::string(
R"(//Description: Sets how fast the max hold markers decrease in value, affecting the speed at which the peak indicators move downward.
//Default value: 900
)");
}

std::string DynamicMaxHoldSecondarySpeedOfFalling::getInfo()
{
    return std::string(
        R"(//Description: Sets how fast the secondary max hold markers decrease in value, affecting the speed at which the peak indicators move downward.
//Default value: 1000
)");
}

std::string DynamicMaxHoldAccelerationStateOfFalling::getInfo()
{
    return std::string(
R"(//Description: Determines whether the max hold elements fall with acceleration or at a constant speed. This gives the user control over the visual behavior and final effect of the peak indicators.
//Default value: True
)");
}

std::string HorizontalLinePositions::getInfo()
{
    return std::string(
R"(//Description: Lets the user define how many horizontal lines are shown on the display and at which signal power levels (in dBFS) they appear. This allows for precise customization of the scale and visual reference points in the signal visualization.
//Default value: –6.02, –12.04, –18.06, ..., –90.30 dBFS
)");
}

std::string VerticalLinePositions::getInfo()
{
    return std::string(
        R"(//Description: Allows selecting the frequency at which a vertical line can be placed.
//Any frequency value can be entered, but it will be automatically adjusted to the nearest
//calculated FFT frequency, in the same way as in the Frequencies.txt file.
)");
}

std::string FrequencyTextPositions::getInfo()
{
    return std::string(
        R"(//Description: Allows selecting the frequency at which a frequency label can be placed.
//Any value can be entered, but it will be automatically adjusted to the nearest
//calculated FFT frequency, as done in the Frequencies.txt file.
)");
}

std::string ColorOfLine::getInfo()
{
    return std::string(
        R"(//Description: Allows you to customize the color (RGB) and transparency of lines that display changing dBFS values on the screen.
//Default value: RGBA color (Red, Green, Blue, Transparency)
)");
}

std::string ColorOfStaticLines::getInfo()
{
    return std::string(
R"(//Description: Allows you to customize the color (RGB) and transparency of static lines that remain constant on the display.
//Default value: RGBA color (Red, Green, Blue, Transparency)
)");
}
std::string ColorOfStaticText::getInfo()
{
    return std::string(
        R"(//Description: Allows you to customize the color (RGB) and transparency of static text that remains constant on the display.
//Default value: RGBA color (Red, Green, Blue, Transparency)
)");
}

std::string ColorsOfRectangle::getInfo()
{
    return std::string(
R"(//Description: Each bar (rectangle) on the screen is made up of 4 corners (vertices). You can set a separate color for each vertex using RGBA values — where R is red, G is green, B is blue, and A stands for transparency. The GPU then smoothly blends the colors between the corners, allowing you to easily control the appearance of each bar, for example by creating smooth color gradients from top to bottom.
//Default value: RGBA color (Red, Green, Blue, Transparency) for each vertex
)");
}

std::string ColorsOfDynamicMaxHoldRectangle::getInfo()
{
    return std::string(
        R"(//Description: Uses the same method of assigning RGBA colors to each of the 4 rectangle vertices as for the main bars. This time, however, it applies to the small visual elements that hold and display the peak (max hold) values over time. By setting the vertex colors, you can control the appearance of these peak indicators, including gradients and transparency.
//Default value: RGBA color (Red, Green, Blue, Transparency) for each vertex
)");
}

std::string ColorOfDynamicMaxHoldLine::getInfo()
{
    return std::string(
        R"(//Description: Allows you to customize the color (RGB) and transparency of dynamic max hold lines that hold and display the peak (max hold) values over time.
//Default value: RGBA color (Red, Green, Blue, Transparency)
)");
}

std::string ColorOfDynamicMaxHoldSecondaryLine::getInfo()
{
    return std::string(
        R"(//Description: Allows you to customize the color (RGB) and transparency of secondary dynamic max hold lines that hold and display the peak (max hold) values over time.
//Default value: RGBA color (Red, Green, Blue, Transparency)
)");
}

std::string ColorsOfDynamicMaxHoldSecondaryRectangle::getInfo()
{
    return std::string(
        R"(//Description: Uses the same method of assigning RGBA colors to each of the 4 rectangle vertices as for the main bars. This time, however, it applies to the secondary visual elements that hold and display the peak (max hold) values over time. By setting the vertex colors, you can control the appearance of these peak indicators, including gradients and transparency.
//Default value: RGBA color (Red, Green, Blue, Transparency) for each vertex)");
}

std::string AdvancedColorSettings::getInfo()
{
    return std::string(
R"(//Description: The application first determines how many frequency bins are defined by the user, based on the number of frequencies provided by the getFrequencies() function. Then, it divides the screen into the same number of vertical bars — one for each frequency. Each bar is built using two triangles (6 vertices), which form a rectangle defined by 4 unique corner points in 2D space (x, y). These bars are then sent to the GPU with their initial positions. At this stage, all bars are evenly spaced and aligned. As the application analyzes the audio (or other signal) in real time, it calculates the power of each frequency. Based on this power, the height or vertical position of each bar is updated dynamically to reflect the current signal strength. The vertex shader updates the positions of the bar's corners depending on the signal’s power for that frequency. The GPU then fills in all the pixels between those corners by generating fragments — and each of these fragments is processed by the fragment shader. One of the values the fragment shader receives is calculatedPosition, which (in simplified terms) tells it how high up the current pixel is within its rectangle. This vertical position is used to control how the pixel will be colored. In addition, the shader receives vertColor values — these are colors assigned to each corner of the rectangle using application-side functions like getColorsOfRectangle() and getColorsOfDynamicMaxHoldRectangle(). The GPU automatically blends these colors across the surface of the rectangle, so each pixel gets its own color depending on where it lies. Inside the fragment shader, this blended color is combined with a vertical color gradient that transitions from blue → cyan → green → yellow → red, depending on the pixel’s height within the bar. This results in vibrant and informative color transitions that follow the signal in real time. Users can adjust the rectangle corner colors directly in the application to experiment with color effects. And because the shader computes colors for each pixel independently, there’s a lot of flexibility for customization. More advanced users can even edit the shader code to fully control how colors behave. All of this runs on the GPU, which executes the fragment shader for every pixel of every bar in parallel. This makes the visualization extremely fast and smooth — even when rendering many bars at once. This method not only results in dynamic and responsive visualizations, but also allows users to experiment and learn how shaders and GPU parallelism work in real-time graphics.
//if you get errors after modification please find following log with error msg: "VS log:"
)");
}

std::string BackgroundColorSettings::getInfo()
{
    return std::string(
        R"()");
}
