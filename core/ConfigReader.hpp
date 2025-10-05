/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "PythonCodeRunner.hpp"
#include <ostream>
#include <vector>

using Frequency = float;
using Position = float;
using Frequencies = std::vector<Frequency>;
using Positions = std::vector<Position>;
using Color = std::vector<float>;
using ColorsOfRectanglePerVertices = std::map<uint32_t, Color>;

struct Configuration
{
    Frequencies frequencies;
    std::vector<float> signalWindow;
    bool pythonDataSourceEnabled;
    bool defaultFullscreenState;
    uint16_t maximizedWindowHorizontalSize;
    uint16_t maximizedWindowVerticalSize;
    uint16_t normalWindowHorizontalSize;
    uint16_t normalWindowVerticalSize;
    uint16_t numberOfRectangles;
    double gapWidthInRelationToRectangleWidth;
    uint32_t numberOfSamples;
    uint32_t samplingRate;
    float desiredFrameRate;
    uint32_t numberOfSignalsForAveraging;
    uint32_t numberOfSignalsForMaxHold;
    float alphaFactor;
    uint32_t maxQueueSize;
    float scalingFactor;
    float offsetFactor;
    bool dynamicMaxHoldVisibilityState;
    float dynamicMaxHoldRectangleHeightInPercentOfScreenSize;
    float dynamicMaxHoldSpeedOfFalling;
    bool dynamicMaxHoldAccelerationStateOfFalling;
    bool advancedColorSettingEnabled;
    Positions horizontalLinePositions;
    Color colorOfStaticLines;
    ColorsOfRectanglePerVertices colorsOfRectangle;
    ColorsOfRectanglePerVertices colorsOfDynamicMaxHoldRectangle;
    std::string advancedColorSettings;
    std::string backgroundColorSettings;
};

std::ostream& operator<<(std::ostream& os, const Configuration & config);


class ConfigReader : public PythonCodeRunner
{
public:
    ConfigReader(const char *moduleName="config");
    Configuration getConfig();
private:
    Frequencies getFrequencies();
    std::vector<float> getSignalWindow();
    bool getPythonDataSourceEnabled();
    bool getDefaultFullscreenState();
    double getMaximizedWindowHorizontalSize();
    double getMaximizedWindowVerticalSize();
    double getNormalWindowHorizontalSize();
    double getNormalWindowVerticalSize();
    double getGapWidthInRelationToRectangleWidth();
    double getNumberOfSamples();
    double getSamplingRate();
    double getDesiredFrameRate();
    double getNumberOfSignalsForAveraging();
    double getNumberOfSignalsForMaxHold();
    double getAlphaFactorForSmoothing();
    double getMaxQueueSize();
    double getScalingFactor();
    double getOffsetFactor();
    bool getDynamicMaxHoldVisibilityState();
    double getDynamicMaxHoldRectangleHeightInPercentOfScreenSize();
    double getDynamicMaxHoldSpeedOfFalling();
    bool getDynamicMaxHoldAccelerationStateOfFalling();
    Positions getHorizontalLinePositions();
    Color getColorOfStaticLines();
    ColorsOfRectanglePerVertices getColorsOfRectangle();
    ColorsOfRectanglePerVertices getColorsOfDynamicMaxHoldRectangle();
    ColorsOfRectanglePerVertices getColorsOfRectangleHelper(const std::string& functionName);
    std::string getAdvancedColorSettings();
    std::string getBackgroundColorSettings();

    Configuration config{};
    bool isConfigReadOut{false};
};

