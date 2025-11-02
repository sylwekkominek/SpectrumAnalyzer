/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <vector>
#include <complex>
#include <cstdint>

std::vector<float> getAverage(const std::vector<float> &left, const std::vector<float> &right);
void zoomData(std::vector<float> &data, const float factor, const float offset);
std::vector<float> normalize(const std::vector<float> &fftData);
std::vector<float> calculatePower(const std::vector<std::complex<float>> &fftData, const float amplitudeCorrection=0, const float offsetFactor=0);
float calculateOverlappingDiff(const uint32_t desiredNumberOfFramesPerSecond, const uint32_t currentFramesPerSecond);
float calculateOverlapping(const uint32_t samplingRate, const uint32_t numberOfSamples, const uint32_t numberOfFramesPerSecond);
std::string formatFloat(float value, int totalWidth, int precision);
std::vector<float> scaleDbfsToPercents(const std::vector<float> &signalInDbfs);
std::vector<float> moveDbFsToPositiveValues(const std::vector<float> &signalInDbfs);
