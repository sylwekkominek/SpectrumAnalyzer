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

using RectangleIndex = uint32_t;
using FrequencyIndex = uint32_t;

using FrequencyIndexesPerRectangle = std::map<RectangleIndex, std::vector<FrequencyIndex>>;

class FrequenciesInfo
{
public:

    FrequenciesInfo(uint32_t samplingRate, uint32_t fftSize, const Frequencies &demandedFrequencies);
    std::vector<RectangleIndex> getRectangleIndexesClosestToFrequencies(const Frequencies &demandedFrequencies);
    uint32_t numberOfFrequencies();
    FrequencyIndexesPerRectangle getAllFrequencyIndexes();
    std::vector<FrequencyRange> getFrequencyRangeForEachRectangle();

private:
    using ClosestFrequency = float;
    using Info = std::pair<FrequencyIndex, Frequency>;

    Frequencies getAllFrequencies() const;
    Info getFrequencyInfo(RectangleIndex rectangleNumber) const;
    void updateAllFrequencies(uint32_t samplingRate, uint32_t fftSize);
    void updateClosestFrequenciesMap(const Frequencies &demandedFrequencies);
    void updateAllFrequenciesMap(const Frequencies &demandedFrequencies);
    void updateContainer(std::set<Frequency> availableFrequencies, const Frequencies &demandedFrequencies, std::function<void(const RectangleIndex &, const Info &)> function);

    uint32_t fftSize;
    uint32_t numberOfRectangles;
    std::set<Frequency> allFrequencies;
    std::map<RectangleIndex, Info> closestFrequenciesMap;
    std::map<RectangleIndex, std::vector<Info>> allFrequenciesMap;
    std::vector<float> selectedData;
};


