/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "PythonCodeRunner.hpp"
#include <ostream>
#include <vector>

using Frequency = float;
using Frequencies = std::vector<Frequency>;
using ColorsOrRectanglePerVertices = std::map<uint, std::vector<float>>;

struct Configuration
{
    Frequencies frequencies;
    uint horizontalSize;
    uint verticalSize;
    uint numberOfRectangles;
    uint numberOfSamples;
    uint samplingRate;
    float overlapping;
    uint numberOfSignalsForAveraging;
    uint numberOfSignalsForMaxHold;
    uint maxQueueSize;
    float scalingFactor;
    float offsetFactor;
    bool smallRectanglesVisibilityState;
    float speedOfFalling;
    bool accelerationStateOfFalling;
    ColorsOrRectanglePerVertices colorsOfRectangle;
    ColorsOrRectanglePerVertices colorsOfSmallRectangle;
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
    double getNumberOfSamples();
    double getSamplingRate();
    double getOverlapping();
    double getNumberOfSignalsForAveraging();
    double getNumberOfSignalsForMaxHold();
    double getMaxQueueSize();
    double getScalingFactor();
    double getOffsetFactor();
    bool getSmallRectanglesVisibilityState();
    double getSpeedOfFalling();
    bool getAccelerationStateOfFalling();
    ColorsOrRectanglePerVertices getColorsOfRectangle();
    ColorsOrRectanglePerVertices getColorsOfSmallRectangle();
    ColorsOrRectanglePerVertices getColorsOfRectangleHelper(const std::string& functionName);

    Configuration config{};
    bool isConfigReadOut{false};
};

