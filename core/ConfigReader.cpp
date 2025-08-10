/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "ConfigReader.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Configuration & config)
{
    auto colorPrinter = [](const auto &color)
    {
        for(auto & component: color)
        {
            std::cout<<component<<" ";
        }
        std::cout<<std::endl;
    };

    auto colorsPrinter = [](const auto &colorsOfRectangle)
    {
        for(auto &[vertex, color]: colorsOfRectangle)
        {
            std::cout<<"vertex: "<<vertex<<" color: ";
            for(auto & component: color)
            {
                std::cout<<component<<" ";
            }
            std::cout<<std::endl;
        }
    };

    os <<"defaultFullscreenState: "<<config.defaultFullscreenState<<std::endl;
    os <<"maximizedWindowHorizontalSize: "<<config.maximizedWindowHorizontalSize<<std::endl;
    os <<"maximizedWindowVerticalSize: "<<config.maximizedWindowVerticalSize<<std::endl;
    os <<"normalWindowHorizontalSize: "<<config.normalWindowHorizontalSize<<std::endl;
    os <<"normalWindowVerticalSize: "<<config.normalWindowVerticalSize<<std::endl;
    os <<"numberOfRectangles: "<<config.numberOfRectangles<<std::endl;
    os <<"gapWidthInRelationToRectangleWidth: "<<config.gapWidthInRelationToRectangleWidth<<std::endl;
    os <<"numberOfSamples: "<<config.numberOfSamples<<std::endl;
    os <<"samplingRate: "<<config.samplingRate<<std::endl;
    os <<"desiredFrameRate: "<<config.desiredFrameRate<<std::endl;
    os <<"numberOfSignalsForAveraging: "<<config.numberOfSignalsForAveraging<<std::endl;
    os <<"numberOfSignalsForMaxHold: "<<config.numberOfSignalsForMaxHold<<std::endl;
    os <<"alphaFactor: "<<config.alphaFactor<<std::endl;
    os <<"maxQueueSize: "<<config.maxQueueSize<<std::endl;
    os <<"scalingFactor: "<<config.scalingFactor<<std::endl;
    os <<"offsetFactor: "<<config.offsetFactor<<std::endl;
    os <<"dynamicMaxHoldVisibilityState: "<<config.dynamicMaxHoldVisibilityState<<std::endl;
    os <<"dynamicMaxHoldRectangleHeightInPercentOfScreenSize: "<<config.dynamicMaxHoldRectangleHeightInPercentOfScreenSize<<std::endl;
    os <<"dynamicMaxHoldSpeedOfFalling: "<<config.dynamicMaxHoldSpeedOfFalling<<std::endl;
    os <<"dynamicMaxHoldAccelerationStateOfFalling: "<<config.dynamicMaxHoldAccelerationStateOfFalling<<std::endl;
    os <<"frequencies: ";

    for(const auto &el: config.frequencies)
    {
        os <<el<<" ";
    }
    os <<std::endl;

    os <<"signalWindow size: "<<config.signalWindow.size()<<std::endl;


    os <<"horizontalLinePositions: ";

    for(const auto &el: config.horizontalLinePositions)
    {
        os <<el<<" ";
    }
    os <<std::endl;

    os <<"colorOfStaticLines: "<<std::endl;
    colorPrinter(config.colorOfStaticLines);
    os <<"colorsOfRectangle: "<<std::endl;
    colorsPrinter(config.colorsOfRectangle);
    os <<"colorsOfDynamicMaxHoldRectangle: "<<std::endl;
    colorsPrinter(config.colorsOfDynamicMaxHoldRectangle);

    return os;
}

ConfigReader::ConfigReader(const char *moduleName):
    PythonCodeRunner(moduleName,
                {"getDefaultFullscreenState",
                 "getMaximizedWindowHorizontalSize",
                 "getMaximizedWindowVerticalSize",
                 "getNormalWindowHorizontalSize",
                 "getNormalWindowVerticalSize",
                 "getGapWidthInRelationToRectangleWidth",
                 "getNumberOfSamples",
                 "getSamplingRate",
                 "getDesiredFrameRate",
                 "getNumberOfSignalsForAveraging",
                 "getNumberOfSignalsForMaxHold",
                 "getAlphaFactorForSmoothing",
                 "getMaxQueueSize",
                 "getFrequencies",
                 "getSignalWindow",
                 "getScalingFactor",
                 "getOffsetFactor",
                 "getDynamicMaxHoldVisibilityState",
                 "getDynamicMaxHoldRectangleHeightInPercentOfScreenSize",
                 "getDynamicMaxHoldSpeedOfFalling",
                 "getDynamicMaxHoldAccelerationStateOfFalling",
                 "getHorizontalLinePositions",
                 "getColorOfStaticLines",
                 "getColorsOfRectangle",
                 "getColorsOfDynamicMaxHoldRectangle",
                 "getAdvancedColorSettings",
                 "getBackgroundColorSettings"
             })
{
}

Configuration ConfigReader::getConfig()
{
    if(not isConfigReadOut)
    {
        config.defaultFullscreenState = getDefaultFullscreenState();
        config.maximizedWindowHorizontalSize = getMaximizedWindowHorizontalSize();
        config.maximizedWindowVerticalSize = getMaximizedWindowVerticalSize();
        config.normalWindowHorizontalSize = getNormalWindowHorizontalSize();
        config.normalWindowVerticalSize = getNormalWindowVerticalSize();
        config.gapWidthInRelationToRectangleWidth = getGapWidthInRelationToRectangleWidth();
        config.numberOfSamples = getNumberOfSamples();
        config.samplingRate = getSamplingRate();
        config.desiredFrameRate = getDesiredFrameRate();
        config.numberOfSignalsForAveraging = getNumberOfSignalsForAveraging();
        config.numberOfSignalsForMaxHold = getNumberOfSignalsForMaxHold();
        config.alphaFactor = getAlphaFactorForSmoothing();
        config.maxQueueSize = getMaxQueueSize();
        config.frequencies = getFrequencies();
        config.signalWindow = getSignalWindow();
        config.numberOfRectangles = config.frequencies.size();
        config.scalingFactor = getScalingFactor();
        config.offsetFactor = getOffsetFactor();
        config.dynamicMaxHoldVisibilityState = getDynamicMaxHoldVisibilityState();
        config.dynamicMaxHoldRectangleHeightInPercentOfScreenSize = getDynamicMaxHoldRectangleHeightInPercentOfScreenSize();
        config.dynamicMaxHoldSpeedOfFalling = getDynamicMaxHoldSpeedOfFalling();
        config.dynamicMaxHoldAccelerationStateOfFalling = getDynamicMaxHoldAccelerationStateOfFalling();
        config.horizontalLinePositions = getHorizontalLinePositions();
        config.colorOfStaticLines = getColorOfStaticLines();
        config.colorsOfRectangle =  getColorsOfRectangle();
        config.colorsOfDynamicMaxHoldRectangle =  getColorsOfDynamicMaxHoldRectangle();
        config.advancedColorSettings = getAdvancedColorSettings();
        config.backgroundColorSettings = getBackgroundColorSettings();
        closePython();
        isConfigReadOut = true;
    }

    return config;
}


Frequencies ConfigReader::getFrequencies()
{
    return getValues(pointersToPythonFunctions.at(__func__));
}

std::vector<float> ConfigReader::getSignalWindow()
{
    return getValues(pointersToPythonFunctions.at(__func__));
}

bool ConfigReader::getDefaultFullscreenState()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getMaximizedWindowHorizontalSize()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getMaximizedWindowVerticalSize()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getNormalWindowHorizontalSize()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getNormalWindowVerticalSize()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getGapWidthInRelationToRectangleWidth()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getNumberOfSamples()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getSamplingRate()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getDesiredFrameRate()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getNumberOfSignalsForAveraging()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getNumberOfSignalsForMaxHold()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getAlphaFactorForSmoothing()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getMaxQueueSize()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getScalingFactor()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getOffsetFactor()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

bool ConfigReader::getDynamicMaxHoldVisibilityState()
{
    return getBooleanValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getDynamicMaxHoldRectangleHeightInPercentOfScreenSize()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getDynamicMaxHoldSpeedOfFalling()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

bool ConfigReader::getDynamicMaxHoldAccelerationStateOfFalling()
{
    return getBooleanValue(pointersToPythonFunctions.at(__func__));
}

Positions ConfigReader::getHorizontalLinePositions()
{
    return getValues(pointersToPythonFunctions.at(__func__));
}

std::vector<float> ConfigReader::getColorOfStaticLines()
{
    return getValues(pointersToPythonFunctions.at(__func__));
}

ColorsOfRectanglePerVertices ConfigReader::getColorsOfRectangle()
{
    return getColorsOfRectangleHelper(__func__);
}

ColorsOfRectanglePerVertices ConfigReader::getColorsOfDynamicMaxHoldRectangle()
{
    return getColorsOfRectangleHelper(__func__);
}

std::string ConfigReader::getAdvancedColorSettings()
{
    return getStringValue(pointersToPythonFunctions.at(__func__));
}

std::string ConfigReader::getBackgroundColorSettings()
{
    return getStringValue(pointersToPythonFunctions.at(__func__));
}

ColorsOfRectanglePerVertices ConfigReader::getColorsOfRectangleHelper(const std::string& functionName)
{
    uint numberOfVerticesInRectangle{4};

    ColorsOfRectanglePerVertices colorsPerVertexNumber{};

    for(uint vertex=0;vertex< numberOfVerticesInRectangle;++vertex)
    {
        auto vertexPyValue = PyLong_FromLong(vertex);
        auto pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, vertexPyValue);
        auto colors = getValues(pointersToPythonFunctions.at(functionName), pArgs);
        Py_DECREF(pArgs);
        colorsPerVertexNumber.emplace(vertex, colors);
    }

    return colorsPerVertexNumber;
}
