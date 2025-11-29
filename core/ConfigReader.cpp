/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "ConfigReader.hpp"

ConfigReader::ConfigReader(const std::string &path) : ConfigFileReader(path)
{
}

Configuration ConfigReader::getConfig()
{
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
        config.data.add(getDynamicMaxHoldVisibilityState());
        config.data.add(getDynamicMaxHoldRectangleHeightInPercentOfScreenSize());
        config.data.add(getDynamicMaxHoldSpeedOfFalling());
        config.data.add(getDynamicMaxHoldAccelerationStateOfFalling());
        config.data.add(getHorizontalLinePositions());
        config.data.add(getColorOfStaticLines());
        config.data.add(getColorsOfRectangle());
        config.data.add(getColorsOfDynamicMaxHoldRectangle());
        config.data.add(getAdvancedColorSettings());
        config.data.add(getBackgroundColorSettings());
    }

    return config;
}

Freqs ConfigReader::getFrequencies()
{
    Freqs data;

    try
    {
        const auto fileName = "Frequencies";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), data.value, 1);
        }
        else
        {
            data.value = readCSVToVector(fileName);
        }
    }
    catch(...)
    {
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
        const auto fileName = "SignalWindow";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), data.value);
            return data;
        }

        const auto dataFromFile = readCSVToVector(fileName);

        if(dataFromFile.size() != numberOfSamples)
        {
            writeVectorToCsv(fileName, data.getInfo(), data.value);
        }
        else
        {
            data.value = readCSVToVector(fileName);
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

    try
    {
        const auto fileName = "PythonDataSourceEnabled";

        if(not checkIfFileExists(fileName))
        {
            writeBoolToFile(fileName, data.getInfo(), data.value);
        }
        else
        {
            data.value = readBoolFromFile(fileName);
        }
    }
    catch(...)
    {
    }

    return data;
}

DefaultFullscreenState ConfigReader::getDefaultFullscreenState()
{
    DefaultFullscreenState data;

    try
    {
        const auto fileName = "DefaultFullscreenState";

        if(not checkIfFileExists(fileName))
        {
            writeBoolToFile(fileName, data.getInfo(), data.value);
        }
        else
        {
            data.value = readBoolFromFile(fileName);
        }
    }
    catch(...)
    {
    }

    return data;
}

MaximizedWindowSize ConfigReader::getMaximizedWindowSize()
{
    MaximizedWindowSize data;

    try
    {
        const auto fileName = "MaximizedWindowSize";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), {(float)data.value.first, (float)data.value.second},0);
        }
        else
        {
            const auto res = readCSVToVector(fileName);
            data.value.first = res.at(0);
            data.value.second = res.at(1);
        }
    }
    catch(...)
    {
    }

    return data;
}

NormalWindowSize ConfigReader::getNormalWindowSize()
{
    NormalWindowSize data;

    try
    {
        const auto fileName = "NormalWindowSize";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), {(float)data.value.first, (float)data.value.second},0);
        }
        else
        {
            const auto res = readCSVToVector(fileName);
            data.value.first = res.at(0);
            data.value.second = res.at(1);
        }
    }
    catch(...)
    {
    }

    return data;
}

GapWidthInRelationToRectangleWidth ConfigReader::getGapWidthInRelationToRectangleWidth()
{
    GapWidthInRelationToRectangleWidth data;

    try
    {
        const auto fileName = "GapWidthInRelationToRectangleWidth";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), {data.value});
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }
    }
    catch(...)
    {
    }

    return data;
}

NumberOfSamples ConfigReader::getNumberOfSamples()
{
    NumberOfSamples data;

    try
    {
        const auto fileName = "NumberOfSamples";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(),{(float)data.value},0);
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }
    }
    catch(...)
    {
    }

    return data;
}

SamplingRate ConfigReader::getSamplingRate()
{
    SamplingRate data;

    try
    {
        const auto fileName = "SamplingRate";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(),{(float)data.value},0);
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }

    }
    catch(...)
    {
    }

    return data;
}

DesiredFrameRate ConfigReader::getDesiredFrameRate()
{
    DesiredFrameRate data;
    try
    {
        const auto fileName = "DesiredFrameRate";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(),{(float)data.value},0);
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }
    }
    catch(...)
    {
    }

    return data;
}

NumberOfSignalsForAveraging ConfigReader::getNumberOfSignalsForAveraging()
{
    NumberOfSignalsForAveraging data;

    try
    {
        const auto fileName = "NumberOfSignalsForAveraging";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(),{(float)data.value},0);
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }
    }
    catch(...)
    {
    }

    return data;
}

NumberOfSignalsForMaxHold ConfigReader::getNumberOfSignalsForMaxHold()
{
    NumberOfSignalsForMaxHold data;

    try
    {
        const auto fileName = "NumberOfSignalsForMaxHold";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(),{(float)data.value},0);
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }
    }
    catch(...)
    {
    }

    return data;
}

AlphaFactor ConfigReader::getAlphaFactorForSmoothing()
{
    AlphaFactor data;

    try
    {
        const auto fileName = "AlphaFactor";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), {data.value});
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }
    }
    catch(...)
    {
    }

    return data;
}

MaxQueueSize ConfigReader::getMaxQueueSize()
{
    MaxQueueSize data;

    try
    {
        const auto fileName = "MaxQueueSize";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), {(float)data.value},0);
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }
    }
    catch(...)
    {
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

    try
    {
        const auto fileName = "OffsetFactor";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), {data.value});
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }
    }
    catch(...)
    {
    }

    return data;
}

DynamicMaxHoldVisibilityState ConfigReader::getDynamicMaxHoldVisibilityState()
{
    DynamicMaxHoldVisibilityState data;

    try
    {
        const auto fileName = "DynamicMaxHoldVisibilityState";

        if(not checkIfFileExists(fileName))
        {
            writeBoolToFile(fileName, data.getInfo(), data.value);
        }
        else
        {
            data.value = readBoolFromFile(fileName);
        }
    }
    catch(...)
    {
    }

    return data;
}

DynamicMaxHoldRectangleHeightInPercentOfScreenSize ConfigReader::getDynamicMaxHoldRectangleHeightInPercentOfScreenSize()
{
    DynamicMaxHoldRectangleHeightInPercentOfScreenSize data;

    try
    {
        const auto fileName = "DynamicMaxHoldRectangleHeightInPercentOfScreenSize";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), {data.value});
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }
    }
    catch(...)
    {
    }

    return data;
}

DynamicMaxHoldSpeedOfFalling ConfigReader::getDynamicMaxHoldSpeedOfFalling()
{
    DynamicMaxHoldSpeedOfFalling data;

    try
    {
        const auto fileName = "DynamicMaxHoldSpeedOfFalling";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), {data.value});
        }
        else
        {
            data.value = readCSVToVector(fileName).at(0);
        }
    }
    catch(...)
    {
    }

    return data;
}

DynamicMaxHoldAccelerationStateOfFalling ConfigReader::getDynamicMaxHoldAccelerationStateOfFalling()
{
    DynamicMaxHoldAccelerationStateOfFalling data;

    try
    {
        const auto fileName = "DynamicMaxHoldAccelerationStateOfFalling";

        if(not checkIfFileExists(fileName))
        {
            writeBoolToFile(fileName, data.getInfo(), data.value);
        }
        else
        {
            data.value = readBoolFromFile(fileName);
        }
    }
    catch(...)
    {
    }

    return data;
}

HorizontalLinePositions ConfigReader::getHorizontalLinePositions()
{
    HorizontalLinePositions data;

    try
    {
        const auto fileName = "HorizontalLinePositions";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), data.value,2);
        }
        else
        {
            data.value = readCSVToVector(fileName);
        }
    }
    catch(...)
    {
    }

    return data;
}

ColorOfStaticLines ConfigReader::getColorOfStaticLines()
{
    ColorOfStaticLines data;

    try
    {
        const auto fileName = "ColorOfStaticLines";

        if(not checkIfFileExists(fileName))
        {
            writeVectorToCsv(fileName, data.getInfo(), data.value,2);
        }
        else
        {
            data.value = readCSVToVector(fileName);
        }
    }
    catch(...)
    {
    }

    return data;
}

ColorsOfRectangle ConfigReader::getColorsOfRectangle()
{
    ColorsOfRectangle data;

    try
    {
        const auto fileName = "ColorsOfRectangle";

        if(not checkIfFileExists(fileName))
        {
            writeMapToCsv(fileName, data.getInfo(), data.value, 2);
        }
        else
        {
            data.value = readCsvToMap(fileName);
        }

    }
    catch(...)
    {
    }

    return data;
}

ColorsOfDynamicMaxHoldRectangle ConfigReader::getColorsOfDynamicMaxHoldRectangle()
{
    ColorsOfDynamicMaxHoldRectangle data;

    try
    {
        const auto fileName = "ColorsOfDynamicMaxHoldRectangle";

        if(not checkIfFileExists(fileName))
        {
            writeMapToCsv(fileName, data.getInfo(), data.value, 2);
        }
        else
        {
            data.value = readCsvToMap(fileName);
        }
    }
    catch(...)
    {
    }

    return data;
}

AdvancedColorSettings ConfigReader::getAdvancedColorSettings()
{
    AdvancedColorSettings data;

    try
    {
        const auto fileName = "AdvancedColorSettings";

        if(not checkIfFileExists(fileName))
        {
            writeStringToFile(fileName, data.getInfo(), data.value);
        }
        else
        {
            data.value = readWholeFile(fileName);
        }
    }
    catch(...)
    {
    }

    return data;
}

BackgroundColorSettings ConfigReader::getBackgroundColorSettings()
{
    BackgroundColorSettings data;

    try
    {
        const auto fileName = "BackgroundColorSettings";

        if(not checkIfFileExists(fileName))
        {
            writeStringToFile(fileName, data.getInfo(), data.value);
        }
        else
        {
            data.value = readWholeFile(fileName);
        }
    }
    catch(...)
    {
    }

    return data;
}






