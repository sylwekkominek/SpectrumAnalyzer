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
using Frequencies = std::vector<Frequency>;
using ColorsOfRectanglePerVertices = std::map<uint, std::vector<float>>;

struct Configuration
{
    Frequencies frequencies;
    uint16_t horizontalSize;
    uint16_t verticalSize;
    uint16_t numberOfRectangles;
    double gapWidthInRelationToRectangleWidth;
    uint numberOfSamples;
    uint samplingRate;
    float overlapping;
    uint numberOfSignalsForAveraging;
    uint numberOfSignalsForMaxHold;
    float alphaFactor;
    uint maxQueueSize;
    float scalingFactor;
    float offsetFactor;
    bool smallRectanglesVisibilityState;
    float smallRectangleHeightInPercentOfScreenSize;
    float speedOfFalling;
    bool accelerationStateOfFalling;
    bool advancedColorSettingEnabled;
    ColorsOfRectanglePerVertices colorsOfRectangle;
    ColorsOfRectanglePerVertices colorsOfSmallRectangle;
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
    double getHorizontalSize();
    double getVerticalSize();
    double getGapWidthInRelationToRectangleWidth();
    double getNumberOfSamples();
    double getSamplingRate();
    double getOverlapping();
    double getNumberOfSignalsForAveraging();
    double getNumberOfSignalsForMaxHold();
    double getAlphaFactorForSmoothing();
    double getMaxQueueSize();
    double getScalingFactor();
    double getOffsetFactor();
    bool getSmallRectanglesVisibilityState();
    double getSmallRectangleHeightInPercentOfScreenSize();
    double getSpeedOfFalling();
    bool getAccelerationStateOfFalling();
    ColorsOfRectanglePerVertices getColorsOfRectangle();
    ColorsOfRectanglePerVertices getColorsOfSmallRectangle();
    ColorsOfRectanglePerVertices getColorsOfRectangleHelper(const std::string& functionName);
    std::string getAdvancedColorSettings();

    Configuration config{};
    bool isConfigReadOut{false};
};

