/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "FrequenciesInfo.hpp"
#include <complex>


class FftBinCombiner
{
public:

    FftBinCombiner(const float scalingFactor, const float offsetFactor, const FrequencyIndexesPerRectangle &data);
    std::vector<float> combineMagnitudes(const std::vector<std::complex<float>> &magnitudes);
    float combineRmsValues(const std::vector<std::complex<float>>& data);
    virtual ~FftBinCombiner()=default;

protected:

    std::vector<float> calculateMagnitude(const std::vector<std::complex<float>>& data);
    std::vector<float> linearToDbfs(const std::vector<float> &data);
    std::vector<float> averageMagnitudeInSpectrum(const std::vector<float> &data);
    std::vector<double> powerInSpectrum(const std::vector<float> &data);

    const float scalingFactor;
    const float offsetFactor;
    const FrequencyIndexesPerRectangle frequencyIndexesPerRectangle;
};
