/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "CommonTypes.hpp"
#include <vector>
#include <cstdint>
#include <map>
#include <string>


using Frequency = float;
using Position = float;
using Frequencies = std::vector<Frequency>;
using Positions = std::vector<Position>;
using Color = std::vector<float>;
using ColorsOfRectanglePerVertices = std::map<uint32_t, Color>;


struct Configuration
{
    template<typename T>
    const auto& get() const
    {
        return data.get<T>().value;
    }

    AnyData data;
};

std::ostream& operator<<(std::ostream& os, const Configuration &config);

struct Freqs
{
    std::string getInfo();
    Frequencies value{20,40,60,80,100,120,150,180,220,250,300,330,360,400,440, 480,520,560,600,720,840,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900, 2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000,3100,3200,3300,3400,3500,3600,3700,3800,3900,4000,4100,4200,4300,4400,4500,4600,4700,4800,4900,5000,5100,5200,5300,5400,5500,5600,5700,5800,5900,6000,6100,6200,6300,6400,6500,6600,6700,6800,6900,7000,7100,7200,7300,7400,7500,7600,7700,7800,7900,8000};
};

struct SignalWindow
{
    SignalWindow(uint16_t numberOfSamples);
    SignalWindow(const std::vector<float> &window);
    std::vector<float> getSignalWindow(uint16_t numberOfSamples);
    std::string getInfo();
    std::vector<float> value;
};

struct PythonDataSourceEnabled
{
    std::string getInfo();
    bool value{false};
};

struct DefaultFullscreenState
{
    std::string getInfo();
    bool value{false};
};

struct NumberOfRectangles
{
    std::string getInfo();
    uint16_t value{92};
};

struct GapWidthInRelationToRectangleWidth
{
    std::string getInfo();
    float value{0};
};

struct MaximizedWindowSize
{
    std::string getInfo();
    std::pair<uint16_t, uint16_t> value{1920, 1080};
};

struct NormalWindowSize
{
    std::string getInfo();
    std::pair<uint16_t, uint16_t> value{1280, 512};
};

struct NumberOfSamples
{
    std::string getInfo();
    uint32_t value{4096};
};

struct SamplingRate
{
    std::string getInfo();
    uint32_t value{44100};
};

struct DesiredFrameRate
{
    std::string getInfo();
    uint32_t value{60};
};

struct NumberOfSignalsForAveraging
{
    std::string getInfo();
    uint32_t value{1};
};

struct NumberOfSignalsForMaxHold
{
    std::string getInfo();
    uint32_t value{5};
};

struct AlphaFactor
{
    std::string getInfo();
    float value{0.25};
};

struct MaxQueueSize
{
    std::string getInfo();
    uint32_t value{20};
};

struct ScalingFactor
{
    ScalingFactor(const float value);
    ScalingFactor(const std::vector<float> &window);
    float getScalingFactor(const std::vector<float> &window);
    std::string getInfo();

    float value;
};

struct OffsetFactor
{
    std::string getInfo();
    float value{0};
};

struct DynamicMaxHoldVisibilityState
{
    std::string getInfo();
    bool value{true};
};

struct DynamicMaxHoldRectangleHeightInPercentOfScreenSize
{
    std::string getInfo();
    float value{0.8};
};

struct DynamicMaxHoldSpeedOfFalling
{
    std::string getInfo();
    float value{900};
};

struct DynamicMaxHoldAccelerationStateOfFalling
{
    std::string getInfo();
    bool value{true};
};

struct HorizontalLinePositions
{
    std::string getInfo();
    Positions value{-6.02, -12.04, -18.06, -24.08, -30.10, -36.12, -36.12, -42.14, -48.16, -54.18, -60.20, -66.22, -72.24, -78.26, -84.28, -90.30};
};

struct ColorOfStaticLines
{
    std::string getInfo();
    Color value{0.15,0.15,0.15,1};
};

struct ColorsOfRectangle
{
    std::string getInfo();
    ColorsOfRectanglePerVertices value{
        {0,{0.2, 0.2, 0.2,0.1}},
        {1,{0,0,0,0.1}},
        {2,{0,0,0,0.1}},
        {3,{0.2, 0.2, 0.2,0.1}}
    };
};

struct ColorsOfDynamicMaxHoldRectangle
{
    std::string getInfo();
    ColorsOfRectanglePerVertices value{
        {0,{0.2, 0.2, 0.2,0.1}},
        {1,{0,0,0,0.1}},
        {2,{0,0,0,0.1}},
        {3,{0.2, 0.2, 0.2,0.1}}
    };
};

struct AdvancedColorSettings
{
    AdvancedColorSettings();
    AdvancedColorSettings(const std::string &shader);
    std::string getAdvancedColorSettings();
    std::string getInfo();
    std::string value;
};

struct BackgroundColorSettings
{
    BackgroundColorSettings();
    BackgroundColorSettings(const std::string &shader);
    std::string getBackgroundColorSettings();
    std::string getInfo();
    std::string value;
};
