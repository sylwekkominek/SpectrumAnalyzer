/*
 * Copyright (C) 2024-2025, Sylwester Kominek
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

bool SamplesCollector::initialize(uint32_t dataLength, uint32_t samplingRate)
{
    try
    {
        auto pArgs = PyTuple_New(2);

        auto dataLengthPyValue = PyLong_FromLong(dataLength);
        auto samplingRatePyValue = PyLong_FromLong(samplingRate);

        PyTuple_SetItem(pArgs, 0, dataLengthPyValue);
        PyTuple_SetItem(pArgs, 1, samplingRatePyValue);

        getValue(pointersToPythonFunctions.at(__func__), pArgs);
        Py_DECREF(pArgs);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

std::vector<float> SamplesCollector::collectDataFromHw()
{
    return getData();
}

SamplesCollector::~SamplesCollector()
{
    Py_Finalize();
}

std::vector<float> SamplesCollector::getData()
{
    return getValues(pointersToPythonFunctions.at(__func__));
}
