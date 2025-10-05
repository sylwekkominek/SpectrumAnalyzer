/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>
#include <complex>

float angleInRadiansToDegrees(float radian);
float angleInDegreesToRadians(float degrees);
float dbFsToAmplitude(float dBFsPower);
std::vector<float> generateSignal(uint32_t numberOfSamples, float samplingFrequency, float singalFrequency, float amplitude=1, float angleInDegrees=0);
std::vector<float> addSignals(const std::vector<float> &signal1, const std::vector<float> &signal2);
std::vector<float> calculatePhase(const std::vector<std::complex<float>> &signal);
std::vector<float> getSignalWindow(uint32_t numberOfSamples);

template <typename T>
std::vector<float> calculateAbs(const std::vector<T> &signal)
{
    std::vector<float> result(signal.size());
    std::transform(signal.begin(), signal.end(), result.begin(), [](const auto &el){ return std::abs(el);});

    return result;
}
