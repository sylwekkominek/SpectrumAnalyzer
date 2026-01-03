/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "DataSourceBase.hpp"
#include "PythonCodeRunner.hpp"
#include <vector>


class PythonDataSource : public PythonCodeRunner, public DataSourceBase
{
public:
    PythonDataSource(const char *moduleName="audioConfig");
    bool initialize(uint32_t numberOfSamples, uint32_t samplingRate) override;
    bool checkIfErrorOccured() override;
    std::vector<float> collectDataFromHw() override;
    ~PythonDataSource();

private:

    std::vector<float> getData();
    bool errorOccured{};

};
