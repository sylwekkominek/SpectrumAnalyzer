/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "SamplesCollectorBase.hpp"
#include "PythonCodeRunner.hpp"
#include <vector>


class SamplesCollector : public PythonCodeRunner, public SamplesCollectorBase
{
public:
    SamplesCollector(const char *moduleName="audioConfig");
    bool initialize(uint32_t numberOfSamples, uint32_t samplingRate) override;
    std::vector<float> collectDataFromHw() override;
    ~SamplesCollector();

private:

    std::vector<float> getData();

};
