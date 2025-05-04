/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "PythonCodeRunner.hpp"
#include <stdexcept>


PythonCodeRunner::PythonCodeRunner(const char *moduleName,  const std::vector<std::string> &pythonFunctionsNames): pythonFunctionsNames(pythonFunctionsNames)
{
    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");

    pName = PyUnicode_DecodeFSDefault(moduleName);

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);


    if (pModule != nullptr)
    {
        updateMapOfPointersToPythonFunctions();

        Py_DECREF(pModule);
    }
    else
    {
      std::string error("Failed to load: ");
      throw std::runtime_error(error+moduleName);
    }
}

PythonCodeRunner::~PythonCodeRunner()
{
    closePython();
}


void PythonCodeRunner::closePython()
{
    Py_Finalize();
}

void PythonCodeRunner::updateMapOfPointersToPythonFunctions()
{
    for(const auto &functionName: pythonFunctionsNames)
    {
        pointersToPythonFunctions.emplace(functionName, getPointerToFunction(functionName.c_str()));
    }
}

PyObject * PythonCodeRunner::getPointerToFunction(const char* functionName)
{
    PyObject *pFunction = PyObject_GetAttrString(pModule, functionName);

    if ((pFunction == nullptr) or (not PyCallable_Check(pFunction)))
    {

      std::string error("Cannot find function: ");
      throw std::runtime_error(error+functionName);

    }
    Py_XDECREF(pFunction);
    return pFunction;
}

double PythonCodeRunner::getValue(PyObject *pointerToPythonFunction, PyObject *args)
{
    double value{};
    PyObject *pValue = PyObject_CallObject(pointerToPythonFunction, args);

    if (pValue != nullptr)
    {
        value = PyFloat_AsDouble(pValue);
        Py_DECREF(pValue);
    }
    else
    {
        Py_DECREF(pointerToPythonFunction);
        Py_DECREF(pModule);

    std::string error("Call failed");
    throw std::runtime_error(error);
    }
    return value;
}

bool PythonCodeRunner::getBooleanValue(PyObject *pointerToPythonFunction)
{
    bool value{};
    PyObject *pValue = PyObject_CallObject(pointerToPythonFunction, nullptr);

    if (pValue != nullptr)
    {
        value = PyObject_IsTrue(pValue);
        Py_DECREF(pValue);
    }
    else
    {
        Py_DECREF(pointerToPythonFunction);
        Py_DECREF(pModule);

    std::string error("Call failed");
    throw std::runtime_error(error);
    }
    return value;
}

std::string PythonCodeRunner::getStringValue(PyObject *pointerToPythonFunction)
{
    std::string value{};
    PyObject *pValue = PyObject_CallObject(pointerToPythonFunction, nullptr);

    if (pValue != nullptr)
    {
        value = std::string(PyUnicode_AsUTF8(pValue));
        Py_DECREF(pValue);
    }
    else
    {
        Py_DECREF(pointerToPythonFunction);
        Py_DECREF(pModule);

        std::string error("Call failed");
        throw std::runtime_error(error);
    }
    return value;
}

std::vector<float> PythonCodeRunner::getValues(PyObject *pointerToPythonFunction, PyObject *args)
{
      PyObject *pValue = PyObject_CallObject(pointerToPythonFunction, args);

      std::vector<float> values;

      if (pValue != nullptr)
      {
        uint32_t dataSize = PyTuple_GET_SIZE(pValue);

        values.reserve(dataSize);

        for(uint i=0;i<dataSize;++i)
        {
            values.push_back(PyFloat_AsDouble(PyTuple_GetItem(pValue, i)));
        }

        Py_DECREF(pValue);
      }
      else
      {
        Py_DECREF(pointerToPythonFunction);
        Py_DECREF(pModule);

        std::string error("Call failed");
        throw std::runtime_error(error);
      }
      return values;
}
