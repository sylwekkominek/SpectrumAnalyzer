/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "ConfigReader.hpp"
#include <iostream>

ConfigReader::ConfigReader(const std::string &path) : ConfigFileReader(path)
{
}

Configuration ConfigReader::getConfig()
{
    std::cout<<"Reading config from: "<<getConfigPath()<<std::endl;

    if(config.data.empty())
    {
        createDirIfNotExists();
        config.data.add(getPythonDataSourceEnabled());
        config.data.add(getDefaultFullscreenState());
        config.data.add(getMaximizedWindowSize());
        config.data.add(getNormalWindowSize());
        config.data.add(getGapWidthInRelationToRectangleWidth());
        config.data.add(getNumberOfSamples());
        config.data.add(getSamplingRate());
        config.data.add(getDesiredFrameRate());
        config.data.add(getNumberOfSignalsForAveraging());
        config.data.add(getNumberOfSignalsForMaxHold());
        config.data.add(getAlphaFactorForSmoothing());
        config.data.add(getMaxQueueSize());
        config.data.add(getFrequencies());
        config.data.add(getSignalWindow());
        config.data.add(getNumberOfRectangles());
        config.data.add(getScalingFactor());
        config.data.add(getOffsetFactor());
        config.data.add(getRectanglesVisibilityState());
        config.data.add(getLinesVisibilityState());
        config.data.add(getDynamicMaxHoldVisibilityState());
        config.data.add(getDynamicMaxHoldSecondaryVisibilityState());
        config.data.add(getDynamicMaxHoldRectangleHeightInPercentOfScreenSize());
        config.data.add(getDynamicMaxHoldSpeedOfFalling());
        config.data.add(getDynamicMaxHoldSecondarySpeedOfFalling());
        config.data.add(getDynamicMaxHoldAccelerationStateOfFalling());
        config.data.add(getHorizontalLinePositions());
        config.data.add(getVerticalLinePositions());
        config.data.add(getFrequencyTextPositions());
        config.data.add(getColorOfLine());
        config.data.add(getColorOfStaticLines());
        config.data.add(getColorOfDynamicMaxHoldLine());
        config.data.add(getColorOfDynamicMaxHoldSecondaryLine());
        config.data.add(getColorsOfRectangle());
        config.data.add(getColorOfDynamicMaxHoldLine());
        config.data.add(getColorsOfDynamicMaxHoldRectangle());
        config.data.add(getColorOfStaticText());
        config.data.add(getColorsOfDynamicMaxHoldSecondaryRectangle());
        config.data.add(getAdvancedColorSettings());
        config.data.add(getBackgroundColorSettings());
    }

    return config;
}


Freqs ConfigReader::getFrequencies()
{
    Freqs data;

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 1);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

NumberOfRectangles ConfigReader::getNumberOfRectangles()
{
    NumberOfRectangles data;

    try
    {
        data.value = getFrequencies().value.size();
    }
    catch(...)
    {
    }

    return data;
}

SignalWindow ConfigReader::getSignalWindow()
{
    auto numberOfSamples = getNumberOfSamples().value;

    SignalWindow data(numberOfSamples);

    try
    {
        if(not checkIfFileExists(data.name))
        {
            writeVectorToCsv(data.name, data.getInfo(), data.value);
            return data;
        }

        const auto dataFromFile = readCSVToVector(data.name);

        if(dataFromFile.size() != numberOfSamples)
        {
            writeVectorToCsv(data.name, data.getInfo(), data.value);
        }
        else
        {
            data.value = readCSVToVector(data.name);
        }
    }
    catch(...)
    {
    }

    return data;
}

PythonDataSourceEnabled ConfigReader::getPythonDataSourceEnabled()
{
    PythonDataSourceEnabled data;

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

DefaultFullscreenState ConfigReader::getDefaultFullscreenState()
{
    DefaultFullscreenState data;

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

MaximizedWindowSize ConfigReader::getMaximizedWindowSize()
{
    MaximizedWindowSize data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value.first, (float)data.value.second},0);

    if(value)
    {
        data.value.first = value->at(0);
        data.value.second = value->at(1);
    }

    return data;
}

NormalWindowSize ConfigReader::getNormalWindowSize()
{
    NormalWindowSize data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value.first, (float)data.value.second},0);

    if(value)
    {
        data.value.first = value->at(0);
        data.value.second = value->at(1);
    }

    return data;
}

GapWidthInRelationToRectangleWidth ConfigReader::getGapWidthInRelationToRectangleWidth()
{
    GapWidthInRelationToRectangleWidth data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {data.value}, 8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

NumberOfSamples ConfigReader::getNumberOfSamples()
{
    NumberOfSamples data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value}, 0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

SamplingRate ConfigReader::getSamplingRate()
{
    SamplingRate data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value}, 0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

DesiredFrameRate ConfigReader::getDesiredFrameRate()
{
    DesiredFrameRate data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value}, 0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

NumberOfSignalsForAveraging ConfigReader::getNumberOfSignalsForAveraging()
{
    NumberOfSignalsForAveraging data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value}, 0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

NumberOfSignalsForMaxHold ConfigReader::getNumberOfSignalsForMaxHold()
{
    NumberOfSignalsForMaxHold data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value}, 0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

AlphaFactor ConfigReader::getAlphaFactorForSmoothing()
{
    AlphaFactor data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value},8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

MaxQueueSize ConfigReader::getMaxQueueSize()
{
    MaxQueueSize data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value},0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

ScalingFactor ConfigReader::getScalingFactor()
{
    ScalingFactor data(getSignalWindow().value);

    // Reading from file is intentionally ignored

    return data;
}

OffsetFactor ConfigReader::getOffsetFactor()
{
    OffsetFactor data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value},8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

RectanglesVisibilityState ConfigReader::getRectanglesVisibilityState()
{
    RectanglesVisibilityState data;

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

LinesVisibilityState ConfigReader::getLinesVisibilityState()
{
    LinesVisibilityState data;

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

DynamicMaxHoldVisibilityState ConfigReader::getDynamicMaxHoldVisibilityState()
{
    DynamicMaxHoldVisibilityState data;

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

DynamicMaxHoldSecondaryVisibilityState ConfigReader::getDynamicMaxHoldSecondaryVisibilityState()
{
    DynamicMaxHoldSecondaryVisibilityState data;

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

DynamicMaxHoldRectangleHeightInPercentOfScreenSize ConfigReader::getDynamicMaxHoldRectangleHeightInPercentOfScreenSize()
{
    DynamicMaxHoldRectangleHeightInPercentOfScreenSize data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {data.value},8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

DynamicMaxHoldSpeedOfFalling ConfigReader::getDynamicMaxHoldSpeedOfFalling()
{
    DynamicMaxHoldSpeedOfFalling data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {data.value},8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

DynamicMaxHoldSecondarySpeedOfFalling ConfigReader::getDynamicMaxHoldSecondarySpeedOfFalling()
{
    DynamicMaxHoldSecondarySpeedOfFalling data;

    auto value = loadVectorConfig(data.name, data.getInfo(), {data.value},8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

DynamicMaxHoldAccelerationStateOfFalling ConfigReader::getDynamicMaxHoldAccelerationStateOfFalling()
{
    DynamicMaxHoldAccelerationStateOfFalling data;

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

HorizontalLinePositions ConfigReader::getHorizontalLinePositions()
{
    HorizontalLinePositions data;

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

VerticalLinePositions ConfigReader::getVerticalLinePositions()
{
    VerticalLinePositions data;

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

FrequencyTextPositions ConfigReader::getFrequencyTextPositions()
{
    FrequencyTextPositions data;

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorOfLine ConfigReader::getColorOfLine()
{
    ColorOfLine data;

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorOfStaticLines ConfigReader::getColorOfStaticLines()
{
    ColorOfStaticLines data;

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorOfStaticText ConfigReader::getColorOfStaticText()
{
    ColorOfStaticText data;

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorsOfRectangle ConfigReader::getColorsOfRectangle()
{
    ColorsOfRectangle data;

    auto value = loadMapConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorOfDynamicMaxHoldLine ConfigReader::getColorOfDynamicMaxHoldLine()
{
    ColorOfDynamicMaxHoldLine data;

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorOfDynamicMaxHoldSecondaryLine ConfigReader::getColorOfDynamicMaxHoldSecondaryLine()
{
    ColorOfDynamicMaxHoldSecondaryLine data;

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorsOfDynamicMaxHoldSecondaryRectangle ConfigReader::getColorsOfDynamicMaxHoldSecondaryRectangle()
{
    ColorsOfDynamicMaxHoldSecondaryRectangle data;

    auto value = loadMapConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorsOfDynamicMaxHoldRectangle ConfigReader::getColorsOfDynamicMaxHoldRectangle()
{
    ColorsOfDynamicMaxHoldRectangle data;

    auto value = loadMapConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

AdvancedColorSettings ConfigReader::getAdvancedColorSettings()
{
    AdvancedColorSettings data;

    auto value = loadStringConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

BackgroundColorSettings ConfigReader::getBackgroundColorSettings()
{
    BackgroundColorSettings data;

    auto value = loadStringConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}






