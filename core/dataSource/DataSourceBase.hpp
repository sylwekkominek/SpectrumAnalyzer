/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <vector>
#include <cstdint>

class DataSourceBase
{
public:
    DataSourceBase()=default;
    virtual ~DataSourceBase()=default;
    bool virtual initialize(uint32_t numberOfSamples, uint32_t samplingRate)=0;
    bool virtual checkIfErrorOccured()=0;
    std::vector<float> virtual collectDataFromHw()=0;

protected:
    uint8_t static constexpr numberOfChannels{2};
    uint32_t dataLength{0};
};


