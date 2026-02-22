/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "DataExchanger.hpp"
#include "CommonTypes.hpp"
#include <fftw3.h>
#include <vector>
#include <complex>
#include <memory>
#include <deque>
#include <cstdint>

using FFTResult= std::vector<std::complex<float>>;


class FFTCalculatorBase
{
public:
    FFTCalculatorBase(uint32_t size);
    virtual FFTResult calculate(const std::vector<float> &inputData)=0;
    virtual ~FFTCalculatorBase();

protected:
    std::unique_ptr<std::vector<fftw_complex>> outPtr;
    fftw_plan p;
};


class RealFFTCalculator : public FFTCalculatorBase
{
public:
    RealFFTCalculator(uint32_t size);
    FFTResult calculate(const std::vector<float> &inputData) override;
    ~RealFFTCalculator()=default;

private:
    std::unique_ptr<std::vector<double>> inRealPtr;
};

class ComplexFFTCalculator : public FFTCalculatorBase
{
public:
    ComplexFFTCalculator(uint32_t size);
    FFTResult calculate(const std::vector<float> &inputData) override;
    ~ComplexFFTCalculator()=default;

private:
    std::unique_ptr<std::vector<fftw_complex>> inComplexPtr;
};


class WelchCalculator
{
public:
    WelchCalculator(const FftType fftType, const uint32_t fftSize, const float overlapping, const std::vector<float> window);
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
    std::unique_ptr<FFTCalculatorBase> fftCalculator;

};






