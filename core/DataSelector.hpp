/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "ConfigReader.hpp"
#include <set>
#include <map>
#include <cstdint>

class DataSelector
{
public:

    DataSelector(uint32_t samplingRate, uint32_t fftSize, const Frequencies &demandedFrequencies);
    std::vector<float> operator()(const std::vector<float> &data);
    std::vector<float> operator()() const;
private:
    uint32_t getFrequencyIndex(uint32_t frequencyNumber) const;
    void updateIndexes(const Frequencies &demandedFrequencies);


    uint32_t fftSize;
    uint32_t numberOfFrequencies;
    std::set<Frequency> allFrequencies;
    std::map<uint32_t,uint32_t> indexesMap;
    std::vector<float> selectedData;
};

