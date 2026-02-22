/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "FrequenciesInfo.hpp"
#include <cstdint>
#include <complex>


class PowerAverager
{
public:

    PowerAverager(const float scalingFactor, const float offsetFactor, const FrequencyIndexesPerRectangle &data);
    std::vector<float> calculate(const std::vector<std::complex<float>> &magnitudes);
    virtual ~PowerAverager()=default;

protected:

    std::vector<float> calculateMagnitude(const std::vector<std::complex<float>>& data);
    std::vector<float> linearToDbfs(const std::vector<float> &data);
    std::vector<float> averagePowerInSpectrum(const std::vector<float> &data);

    const float scalingFactor;
    const float offsetFactor;
    const FrequencyIndexesPerRectangle frequencyIndexesPerRectangle;
};



