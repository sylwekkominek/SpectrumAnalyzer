/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "ConfigReader.hpp"
#include <iostream>
#include <unordered_set>

ConfigReader::ConfigReader(const ThemeConfig theme, const std::string &path) : ConfigFileReader(theme, path)
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
        config.data.add(getColorOfDynamicMaxHoldSecondaryLine());
        config.data.add(getColorsOfRectangle());
        config.data.add(getColorOfDynamicMaxHoldLine());
        config.data.add(getColorsOfDynamicMaxHoldRectangle());
        config.data.add(getColorOfStaticText());
        config.data.add(getColorsOfDynamicMaxHoldSecondaryRectangle());
        config.data.add(getAdvancedColorSettings());
        config.data.add(getBackgroundColorSettings());
        config.data.add(getWindowTitle());
    }

    return config;
}


Freqs ConfigReader::getFrequencies()
{
    std::unordered_set<Frequency> seen;

    Freqs data(themeConfig, getSamplingRate().value, getNumberOfSamples().value);

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 1);

    if(value)
    {
        for (auto &freq : *value)
        {
            if (!seen.insert(freq).second)
            {
                std::cout<<"WARNING: DUPLICATE FREQUENCY: "<<freq<<std::endl;
            }
        }

        data.value = std::move(*value);
    }

    return data;
}

NumberOfRectangles ConfigReader::getNumberOfRectangles()
{
    NumberOfRectangles data(0);

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
    PythonDataSourceEnabled data(themeConfig);

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

DefaultFullscreenState ConfigReader::getDefaultFullscreenState()
{
    DefaultFullscreenState data(themeConfig);

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

MaximizedWindowSize ConfigReader::getMaximizedWindowSize()
{
    MaximizedWindowSize data(themeConfig);

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
    NormalWindowSize data(themeConfig);

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
    GapWidthInRelationToRectangleWidth data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {data.value}, 8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

NumberOfSamples ConfigReader::getNumberOfSamples()
{
    NumberOfSamples data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value}, 0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

SamplingRate ConfigReader::getSamplingRate()
{
    SamplingRate data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value}, 0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

DesiredFrameRate ConfigReader::getDesiredFrameRate()
{
    DesiredFrameRate data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value}, 0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

NumberOfSignalsForAveraging ConfigReader::getNumberOfSignalsForAveraging()
{
    NumberOfSignalsForAveraging data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value}, 0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

NumberOfSignalsForMaxHold ConfigReader::getNumberOfSignalsForMaxHold()
{
    NumberOfSignalsForMaxHold data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value}, 0);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

AlphaFactor ConfigReader::getAlphaFactorForSmoothing()
{
    AlphaFactor data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value},8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

MaxQueueSize ConfigReader::getMaxQueueSize()
{
    MaxQueueSize data(themeConfig);

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
    OffsetFactor data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {(float)data.value},8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

RectanglesVisibilityState ConfigReader::getRectanglesVisibilityState()
{
    RectanglesVisibilityState data(themeConfig);

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

LinesVisibilityState ConfigReader::getLinesVisibilityState()
{
    LinesVisibilityState data(themeConfig);

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

DynamicMaxHoldVisibilityState ConfigReader::getDynamicMaxHoldVisibilityState()
{
    DynamicMaxHoldVisibilityState data(themeConfig);

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

DynamicMaxHoldSecondaryVisibilityState ConfigReader::getDynamicMaxHoldSecondaryVisibilityState()
{
    DynamicMaxHoldSecondaryVisibilityState data(themeConfig);

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

DynamicMaxHoldRectangleHeightInPercentOfScreenSize ConfigReader::getDynamicMaxHoldRectangleHeightInPercentOfScreenSize()
{
    DynamicMaxHoldRectangleHeightInPercentOfScreenSize data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {data.value},8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

DynamicMaxHoldSpeedOfFalling ConfigReader::getDynamicMaxHoldSpeedOfFalling()
{
    DynamicMaxHoldSpeedOfFalling data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {data.value},8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

DynamicMaxHoldSecondarySpeedOfFalling ConfigReader::getDynamicMaxHoldSecondarySpeedOfFalling()
{
    DynamicMaxHoldSecondarySpeedOfFalling data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), {data.value},8);

    if(value)
    {
        data.value = value->at(0);
    }

    return data;
}

DynamicMaxHoldAccelerationStateOfFalling ConfigReader::getDynamicMaxHoldAccelerationStateOfFalling()
{
    DynamicMaxHoldAccelerationStateOfFalling data(themeConfig);

    auto value = loadBoolConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = *value;
    }

    return data;
}

HorizontalLinePositions ConfigReader::getHorizontalLinePositions()
{
    HorizontalLinePositions data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

VerticalLinePositions ConfigReader::getVerticalLinePositions()
{
    VerticalLinePositions data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

FrequencyTextPositions ConfigReader::getFrequencyTextPositions()
{
    FrequencyTextPositions data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorOfLine ConfigReader::getColorOfLine()
{
    ColorOfLine data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorOfStaticLines ConfigReader::getColorOfStaticLines()
{
    ColorOfStaticLines data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorOfStaticText ConfigReader::getColorOfStaticText()
{
    ColorOfStaticText data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorsOfRectangle ConfigReader::getColorsOfRectangle()
{
    ColorsOfRectangle data(themeConfig);

    auto value = loadMapConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorOfDynamicMaxHoldLine ConfigReader::getColorOfDynamicMaxHoldLine()
{
    ColorOfDynamicMaxHoldLine data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorOfDynamicMaxHoldSecondaryLine ConfigReader::getColorOfDynamicMaxHoldSecondaryLine()
{
    ColorOfDynamicMaxHoldSecondaryLine data(themeConfig);

    auto value = loadVectorConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorsOfDynamicMaxHoldSecondaryRectangle ConfigReader::getColorsOfDynamicMaxHoldSecondaryRectangle()
{
    ColorsOfDynamicMaxHoldSecondaryRectangle data(themeConfig);

    auto value = loadMapConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

ColorsOfDynamicMaxHoldRectangle ConfigReader::getColorsOfDynamicMaxHoldRectangle()
{
    ColorsOfDynamicMaxHoldRectangle data(themeConfig);

    auto value = loadMapConfig(data.name, data.getInfo(), data.value, 2);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

AdvancedColorSettings ConfigReader::getAdvancedColorSettings()
{
    AdvancedColorSettings data(themeConfig);

    auto value = loadStringConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

BackgroundColorSettings ConfigReader::getBackgroundColorSettings()
{
    BackgroundColorSettings data(themeConfig);

    auto value = loadStringConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}

WindowTitle ConfigReader::getWindowTitle()
{
    WindowTitle data(themeConfig);

    auto value = loadStringConfig(data.name, data.getInfo(), data.value);

    if(value)
    {
        data.value = std::move(*value);
    }

    return data;
}






