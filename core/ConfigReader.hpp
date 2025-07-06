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
using ColorsOfRectanglePerVertices = std::map<uint, Color>;

struct Configuration
{
    Frequencies frequencies;
    std::vector<float> signalWindow;
    uint16_t horizontalSize;
    uint16_t verticalSize;
    uint16_t numberOfRectangles;
    double gapWidthInRelationToRectangleWidth;
    uint numberOfSamples;
    uint samplingRate;
    float desiredFrameRate;
    uint numberOfSignalsForAveraging;
    uint numberOfSignalsForMaxHold;
    float alphaFactor;
    uint maxQueueSize;
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
    double getHorizontalSize();
    double getVerticalSize();
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

    Configuration config{};
    bool isConfigReadOut{false};
};

