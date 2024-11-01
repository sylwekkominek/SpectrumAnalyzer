/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <Python.h>
#include <string>
#include <vector>
#include <map>


class PythonCodeRunner
{
public:

    PythonCodeRunner(const char *moduleName,  const std::vector<std::string> &pythonFunctionsNames);
    ~PythonCodeRunner();

protected:

    void closePython();
    void updateMapOfPointersToPythonFunctions();
    PyObject * getPointerToFunction(const char* functionName);
    bool getBooleanValue(PyObject *pointerToPythonFunction);
    double getValue(PyObject *pointerToPythonFunction, PyObject *args=nullptr);
    std::vector<float> getValues(PyObject *pointerToPythonFunction, PyObject *args=nullptr);

    const std::vector<std::string> pythonFunctionsNames;
    std::map<std::string, PyObject *> pointersToPythonFunctions;
    PyObject *pName;
    PyObject *pModule;

};
