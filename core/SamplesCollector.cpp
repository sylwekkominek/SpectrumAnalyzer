/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "SamplesCollector.hpp"
#include <vector>
#include <map>


SamplesCollector::SamplesCollector(const char *moduleName):
    PythonCodeRunner(moduleName,
                {"initialize",
                 "getData"})
{
}

void SamplesCollector::initialize(uint32_t dataLength, uint32_t samplingRate)
{
    auto pArgs = PyTuple_New(2);

    auto dataLengthPyValue = PyLong_FromLong(dataLength);
    auto samplingRatePyValue = PyLong_FromLong(samplingRate);

    PyTuple_SetItem(pArgs, 0, dataLengthPyValue);
    PyTuple_SetItem(pArgs, 1, samplingRatePyValue);

    getValue(pointersToPythonFunctions.at(__func__),pArgs);
    Py_DECREF(pArgs);
}

void SamplesCollector::collectDataFromHw()
{
    auto channels = getData();

    rightChannel.clear();
    leftChannel.clear();


    rightChannel.reserve(channels.size()/2);
    leftChannel.reserve(channels.size()/2);

    for(uint i=0;i<channels.size();++i)
    {
       if(i % 2)
       {
           rightChannel.push_back(channels[i]);
       }
       else
       {
            leftChannel.push_back(channels[i]);
       }
    }
}

std::vector<float> SamplesCollector::getDataFromRightChannel()
{
    return rightChannel;
}

std::vector<float> SamplesCollector::getDataFromLeftChannel()
{
    return leftChannel;
}

SamplesCollector::~SamplesCollector()
{
    Py_Finalize();
}

std::vector<float> SamplesCollector::getData()
{
    return getValues(pointersToPythonFunctions.at(__func__));
}
