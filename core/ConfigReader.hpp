/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "Config.hpp"
#include "ConfigFileReader.hpp"


class ConfigReader : public ConfigFileReader
{
public:
    ConfigReader(const std::string &path="config");
    Configuration getConfig();

private:
    Freqs getFrequencies();
    NumberOfRectangles getNumberOfRectangles();
    SignalWindow getSignalWindow();
    PythonDataSourceEnabled getPythonDataSourceEnabled();
    DefaultFullscreenState getDefaultFullscreenState();
    MaximizedWindowSize getMaximizedWindowSize();
    NormalWindowSize getNormalWindowSize();
    GapWidthInRelationToRectangleWidth getGapWidthInRelationToRectangleWidth();
    NumberOfSamples getNumberOfSamples();
    SamplingRate getSamplingRate();
    DesiredFrameRate getDesiredFrameRate();
    NumberOfSignalsForAveraging getNumberOfSignalsForAveraging();
    NumberOfSignalsForMaxHold getNumberOfSignalsForMaxHold();
    AlphaFactor getAlphaFactorForSmoothing();
    MaxQueueSize getMaxQueueSize();
    ScalingFactor getScalingFactor();
    OffsetFactor getOffsetFactor();
    DynamicMaxHoldVisibilityState getDynamicMaxHoldVisibilityState();
    DynamicMaxHoldRectangleHeightInPercentOfScreenSize getDynamicMaxHoldRectangleHeightInPercentOfScreenSize();
    DynamicMaxHoldSpeedOfFalling getDynamicMaxHoldSpeedOfFalling();
    DynamicMaxHoldAccelerationStateOfFalling getDynamicMaxHoldAccelerationStateOfFalling();
    HorizontalLinePositions getHorizontalLinePositions();
    ColorOfStaticLines getColorOfStaticLines();
    ColorsOfRectangle getColorsOfRectangle();
    ColorsOfDynamicMaxHoldRectangle getColorsOfDynamicMaxHoldRectangle();
    ColorsOfRectanglePerVertices getColorsOfRectangleHelper(const std::string& functionName);
    AdvancedColorSettings getAdvancedColorSettings();
    BackgroundColorSettings getBackgroundColorSettings();

    Configuration config{};
};
