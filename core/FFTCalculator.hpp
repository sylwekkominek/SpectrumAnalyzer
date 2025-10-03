/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "DataExchanger.hpp"
#include <fftw3.h>
#include <vector>
#include <complex>
#include <memory>
#include <deque>
#include <cstdint>

using FFTResult= std::vector<std::complex<float>>;


class FFTCalculator
{
public:
    FFTCalculator(uint32_t size);
    FFTResult calculate(const std::vector<float> &inputData);
    ~FFTCalculator();

private:

    std::unique_ptr<std::vector<fftw_complex>> inPtr;
    std::unique_ptr<std::vector<fftw_complex>> outPtr;
    fftw_plan p;
};

class WelchCalculator :  FFTCalculator
{
public:
    WelchCalculator(const uint32_t fftSize, const float overlapping, const std::vector<float> window);
    void updateBuffer(const std::vector<float> &inputData);
    void updateOverlapping(const float newOverlapping);
    void calculate(DataExchanger<std::unique_ptr<FFTResult>> &queue);

private:
    uint32_t calculateNumberOfSamplesToBeRemoved();

    const uint32_t fftSize;
    float overlapping;
    uint32_t numberOfSamplesToBeRemoved;
    std::deque<float> bufforWithDataToBeConverted;
    const std::vector<float> window;
};






