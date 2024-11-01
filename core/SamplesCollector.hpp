/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "PythonCodeRunner.hpp"
#include <vector>


class SamplesCollector : public PythonCodeRunner
{
public:
    SamplesCollector(const char *moduleName="audioConfig");
    void initialize(uint32_t dataLength, uint32_t samplingRate);
    void collectDataFromHw();
    std::vector<float> getDataFromRightChannel();
    std::vector<float> getDataFromLeftChannel();
    ~SamplesCollector();

private:

    std::vector<float> getData();
    std::vector<float> leftChannel;
    std::vector<float> rightChannel;

};
