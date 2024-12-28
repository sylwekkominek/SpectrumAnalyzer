/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <vector>
#include <complex>

std::vector<float> getAverage(const std::vector<float> &left, const std::vector<float> &right);
void zoomData(std::vector<float> &data, const float factor, const float offset);
std::vector<float> normalize(const std::vector<float> &fftData);
std::vector<float> calculatePower(const std::vector<std::complex<float>> &fftData);






