/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "CommonTypes.hpp"
#include <fftw3.h>
#include <vector>
#include <complex>
#include <memory>
#include <deque>
#include <cstdint>

using FftResult= std::vector<std::complex<float>>;


class FftCalculatorBase
{
public:
    FftCalculatorBase(uint32_t size);
    virtual FftResult calculate(const std::vector<float> &inputData)=0;
    virtual ~FftCalculatorBase();

protected:
    std::unique_ptr<std::vector<fftw_complex>> outPtr;
    fftw_plan p;
};

class RealFftCalculator : public FftCalculatorBase
{
public:
    RealFftCalculator(uint32_t size);
    FftResult calculate(const std::vector<float> &inputData) override;
    ~RealFftCalculator()=default;

private:
    std::unique_ptr<std::vector<double>> inRealPtr;
};

class ComplexFftCalculator : public FftCalculatorBase
{
public:
    ComplexFftCalculator(uint32_t size);
    FftResult calculate(const std::vector<float> &inputData) override;
    ~ComplexFftCalculator()=default;

private:
    std::unique_ptr<std::vector<fftw_complex>> inComplexPtr;
};

class WelchCalculator
{
public:
    WelchCalculator(const FftType fftType, const uint32_t fftSize, const float overlapping, const std::vector<float> window);
    void updateBuffer(const std::vector<float> &inputData);
    void updateOverlapping(const float newOverlapping);
    std::vector<FftResult> calculate();

private:
    uint32_t calculateNumberOfSamplesToBeRemoved();

    const uint32_t fftSize;
    float overlapping;
    uint32_t numberOfSamplesToBeRemoved;
    std::deque<float> bufforWithDataToBeConverted;
    const std::vector<float> window;
    std::unique_ptr<FftCalculatorBase> fftCalculator;
};






