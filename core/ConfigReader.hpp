/*
 * Copyright (C) 2024-2026, Sylwester Kominek
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
    RectanglesVisibilityState getRectanglesVisibilityState();
    LinesVisibilityState getLinesVisibilityState();
    DynamicMaxHoldVisibilityState getDynamicMaxHoldVisibilityState();
    DynamicMaxHoldSecondaryVisibilityState getDynamicMaxHoldSecondaryVisibilityState();
    DynamicMaxHoldRectangleHeightInPercentOfScreenSize getDynamicMaxHoldRectangleHeightInPercentOfScreenSize();
    DynamicMaxHoldSpeedOfFalling getDynamicMaxHoldSpeedOfFalling();
    DynamicMaxHoldSecondarySpeedOfFalling getDynamicMaxHoldSecondarySpeedOfFalling();
    DynamicMaxHoldAccelerationStateOfFalling getDynamicMaxHoldAccelerationStateOfFalling();
    HorizontalLinePositions getHorizontalLinePositions();
    VerticalLinePositions getVerticalLinePositions();
    FrequencyTextPositions getFrequencyTextPositions();
    ColorOfLine getColorOfLine();
    ColorOfStaticLines getColorOfStaticLines();
    ColorOfStaticText getColorOfStaticText();
    ColorsOfRectangle getColorsOfRectangle();
    ColorOfDynamicMaxHoldLine getColorOfDynamicMaxHoldLine();
    ColorOfDynamicMaxHoldSecondaryLine getColorOfDynamicMaxHoldSecondaryLine();
    ColorsOfDynamicMaxHoldRectangle getColorsOfDynamicMaxHoldRectangle();
    ColorsOfDynamicMaxHoldSecondaryRectangle getColorsOfDynamicMaxHoldSecondaryRectangle();
    ColorsOfRectanglePerVertices getColorsOfRectangleHelper(const std::string& functionName);
    AdvancedColorSettings getAdvancedColorSettings();
    BackgroundColorSettings getBackgroundColorSettings();

    Configuration config{};
};
