/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "ConfigReader.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Configuration & config)
{
    os <<"horizontalSize: "<<config.horizontalSize<<std::endl;
    os <<"verticalSize: "<<config.verticalSize<<std::endl;
    os <<"numberOfRectangles: "<<config.numberOfRectangles<<std::endl;
    os <<"numberOfSamples: "<<config.numberOfSamples<<std::endl;
    os <<"samplingRate: "<<config.samplingRate<<std::endl;
    os <<"overlapping: "<<config.overlapping<<std::endl;
    os <<"numberOfSignalsForAveraging: "<<config.numberOfSignalsForAveraging<<std::endl;
    os <<"numberOfSignalsForMaxHold: "<<config.numberOfSignalsForMaxHold<<std::endl;
    os <<"maxQueueSize: "<<config.maxQueueSize<<std::endl;
    os <<"scalingFactor: "<<config.scalingFactor<<std::endl;
    os <<"offsetFactor: "<<config.offsetFactor<<std::endl;
    os <<"smallRectanglesVisibilityState: "<<config.smallRectanglesVisibilityState<<std::endl;
    os <<"speedOfFalling: "<<config.speedOfFalling<<std::endl;
    os <<"accelerationStateOfFalling: "<<config.accelerationStateOfFalling<<std::endl;

    os <<"frequencies: ";

    for(const auto &el: config.frequencies)
    {
        os <<el<<" ";
    }
    os <<std::endl;

    auto colorsPrinter = [](const auto &colorsOfRectangle)
    {
        for(auto &[vertex, colors]: colorsOfRectangle)
        {
            std::cout<<"vertex: "<<vertex<<" colors: ";
            for(auto & color: colors)
            {
                std::cout<<color<<" ";
            }
            std::cout<<std::endl;
        }
    };

    os <<"colorsOfRectangle: "<<std::endl;
    colorsPrinter(config.colorsOfRectangle);
    os <<"colorsOfSmallRectangle: "<<std::endl;
    colorsPrinter(config.colorsOfSmallRectangle);

    return os;
}

ConfigReader::ConfigReader(const char *moduleName):
    PythonCodeRunner(moduleName,
                {"getHorizontalSize",
                 "getVerticalSize",
                 "getNumberOfSamples",
                 "getSamplingRate",
                 "getOverlapping",
                 "getNumberOfSignalsForAveraging",
                 "getNumberOfSignalsForMaxHold",
                 "getMaxQueueSize",
                 "getFrequencies",
                 "getScalingFactor",
                 "getOffsetFactor",
                 "getSmallRectanglesVisibilityState",
                 "getSpeedOfFalling",
                 "getAccelerationStateOfFalling",
                 "getColorsOfRectangle",
                 "getColorsOfSmallRectangle"
             })
{
}

Configuration ConfigReader::getConfig()
{
    if(not isConfigReadOut)
    {
        config.horizontalSize = getHorizontalSize();
        config.verticalSize = getVerticalSize();
        config.numberOfSamples = getNumberOfSamples();
        config.samplingRate = getSamplingRate();
        config.overlapping = getOverlapping();
        config.numberOfSignalsForAveraging = getNumberOfSignalsForAveraging();
        config.numberOfSignalsForMaxHold = getNumberOfSignalsForMaxHold();
        config.maxQueueSize = getMaxQueueSize();
        config.frequencies = getFrequencies();
        config.numberOfRectangles = config.frequencies.size();
        config.scalingFactor = getScalingFactor();
        config.offsetFactor = getOffsetFactor();
        config.smallRectanglesVisibilityState = getSmallRectanglesVisibilityState();
        config.speedOfFalling = getSpeedOfFalling();
        config.accelerationStateOfFalling = getAccelerationStateOfFalling();
        config.colorsOfRectangle =  getColorsOfRectangle();
        config.colorsOfSmallRectangle =  getColorsOfSmallRectangle();
        closePython();
        isConfigReadOut = true;
    }

    return config;
}

Frequencies ConfigReader::getFrequencies()
{
    return getValues(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getHorizontalSize()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getVerticalSize()
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

double ConfigReader::getOverlapping()
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

bool ConfigReader::getSmallRectanglesVisibilityState()
{
    return getBooleanValue(pointersToPythonFunctions.at(__func__));
}

double ConfigReader::getSpeedOfFalling()
{
    return getValue(pointersToPythonFunctions.at(__func__));
}

bool ConfigReader::getAccelerationStateOfFalling()
{
    return getBooleanValue(pointersToPythonFunctions.at(__func__));
}

ColorsOrRectanglePerVertices ConfigReader::getColorsOfRectangle()
{
    return getColorsOfRectangleHelper(__func__);
}

ColorsOrRectanglePerVertices ConfigReader::getColorsOfSmallRectangle()
{
    return getColorsOfRectangleHelper(__func__);
}

ColorsOrRectanglePerVertices ConfigReader::getColorsOfRectangleHelper(const std::string& functionName)
{
    uint numberOfVerticesInRectangle{4};

    ColorsOrRectanglePerVertices colorsPerVertexNumber{};

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
