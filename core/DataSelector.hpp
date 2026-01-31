/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "CommonTypes.hpp"
#include <set>
#include <map>
#include <cstdint>

class DataSelector
{
public:

    DataSelector(uint32_t samplingRate, uint32_t fftSize, const Frequencies &demandedFrequencies);
    std::vector<float> operator()(const std::vector<float> &data);
    std::vector<float> operator()() const;
    Frequencies getSelectedFrequencies() const;
    std::vector<uint16_t> getRectangleIndexesClosestToFrequencies(const Frequencies &demandedFrequencies);

private:

    using RectangleIndex = uint32_t;
    using FrequencyIndex = uint32_t;
    using ClosestFrequency = float;
    using FrequencyInfo = std::pair<FrequencyIndex, Frequency>;

    FrequencyInfo getFrequencyInfo(RectangleIndex rectangleNumber) const;
    void updateIndexes(const Frequencies &demandedFrequencies);
    void updateContainer(std::set<Frequency> availableFrequencies, const Frequencies &demandedFrequencies, std::function<void(const RectangleIndex &, const FrequencyInfo &)> function);

    uint32_t fftSize;
    uint32_t numberOfRectangles;
    std::set<Frequency> allFrequencies;
    std::map<RectangleIndex, FrequencyInfo> indexesMap;
    std::vector<float> selectedData;
};

