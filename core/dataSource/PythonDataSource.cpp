/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "PythonDataSource.hpp"
#include <vector>
#include <map>


PythonDataSource::PythonDataSource(const char *moduleName):
    PythonCodeRunner(moduleName,
                     {"initialize",
                      "getData"})
{
}

bool PythonDataSource::initialize(uint32_t dataLength, uint32_t samplingRate)
{
    errorOccured = false;

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
        errorOccured = true;
        return false;
    }

    return true;
}

std::vector<float> PythonDataSource::collectDataFromHw()
{
    return getData();
}

bool PythonDataSource::checkIfErrorOccured()
{
    return errorOccured;
}

PythonDataSource::~PythonDataSource()
{
    Py_Finalize();
}

std::vector<float> PythonDataSource::getData()
{
    try
    {
        return getValues(pointersToPythonFunctions.at(__func__));
    }
    catch(...)
    {
        errorOccured = true;
    }
    return std::vector<float>{};
}
