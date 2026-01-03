/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "DataSourceBase.hpp"
#include <vector>
#include <memory>
#include <cstdint>
#include <string>

class SamplesCollector
{
public:
    SamplesCollector(const bool pythonDataSourceEnabled, const std::string &audioConfigFile="audioConfig");

    bool initialize(uint32_t numberOfSamples, uint32_t sampleRate);
    bool checkIfErrorOccured();
    std::vector<float> collectDataFromHw();

private:
    std::unique_ptr<DataSourceBase> dataSourceImpl;
};
