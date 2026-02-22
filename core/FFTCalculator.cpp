/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "FFTCalculator.hpp"
#include "DataExchanger.hpp"
#include <cstdint>


FFTCalculatorBase::FFTCalculatorBase(uint32_t size) : outPtr(std::make_unique<std::vector<fftw_complex>>(size))
{
}

RealFFTCalculator::RealFFTCalculator(uint32_t size): FFTCalculatorBase((size / 2)+ 1), inRealPtr(std::make_unique<std::vector<double>>(size))
{
    p = fftw_plan_dft_r2c_1d(size, inRealPtr->data(), outPtr->data(), FFTW_MEASURE);
}

FFTResult RealFFTCalculator::calculate(const std::vector<float> &inputData)
{

    for(uint32_t i =0; i<inRealPtr->size(); ++i)
    {
        (*inRealPtr)[i] = inputData.at(i);
    }

    fftw_execute(p);

    const uint32_t inputSize = inRealPtr->size();
    const uint32_t outputSize = outPtr->size();

    std::vector<std::complex<float>> outputData;
    outputData.reserve(inputSize);

    for (uint32_t i = 0; i < outputSize; ++i)
    {
        outputData.emplace_back((*outPtr)[i][0], (*outPtr)[i][1]);
    }

    for (uint32_t i = 1; i < outputSize - 1; ++i)
    {
        outputData.emplace_back((*outPtr)[outputSize - 1 - i][0], -(*outPtr)[outputSize - 1 - i][1]);
    }

    return outputData;
}


ComplexFFTCalculator::ComplexFFTCalculator(uint32_t size): FFTCalculatorBase(size), inComplexPtr(std::make_unique<std::vector<fftw_complex>>(size))
{
    p = fftw_plan_dft_1d(size, inComplexPtr->data(), outPtr->data(), FFTW_FORWARD,  FFTW_MEASURE);
}

FFTResult ComplexFFTCalculator::calculate(const std::vector<float> &inputData)
{

    for(uint32_t i =0; i<inComplexPtr->size(); ++i)
    {
        (*inComplexPtr)[i][0] = inputData.at(i);
        (*inComplexPtr)[i][1] = 0;
    }

    fftw_execute(p);

    std::vector<std::complex<float>> outputData;
    outputData.reserve(outPtr->size());

    for(uint32_t i =0; i<outPtr->size(); ++i)
    {
        outputData.emplace_back((*outPtr)[i][0],(*outPtr)[i][1]);
    }

    return outputData;
}


FFTCalculatorBase::~FFTCalculatorBase()
{
    fftw_destroy_plan(p);
    fftw_cleanup();
}


WelchCalculator::WelchCalculator(const FftType fftType, const uint32_t fftSize, const float overlapping, const std::vector<float> window) :
    overlapping(overlapping),
    numberOfSamplesToBeRemoved(calculateNumberOfSamplesToBeRemoved()),
    fftSize(fftSize), window(window)
{
    fftCalculator = (fftType == FftType::Complex)
                        ? std::unique_ptr<FFTCalculatorBase>(std::make_unique<ComplexFFTCalculator>(fftSize))
                        : std::unique_ptr<FFTCalculatorBase>(std::make_unique<RealFFTCalculator>(fftSize));
}


void WelchCalculator::updateBuffer(const std::vector<float> &inputData)
{
    bufforWithDataToBeConverted.insert(bufforWithDataToBeConverted.end(), inputData.begin(), inputData.end());
}


void WelchCalculator::updateOverlapping(const float newOverlapping)
{
    overlapping = newOverlapping;
    numberOfSamplesToBeRemoved = calculateNumberOfSamplesToBeRemoved();
}

void WelchCalculator::calculate(DataExchanger<std::unique_ptr<FFTResult>> &queue)
{

    while(bufforWithDataToBeConverted.size() >= fftSize)
    {
        std::vector<float> dataInTimeDomain(bufforWithDataToBeConverted.begin(), bufforWithDataToBeConverted.begin() + fftSize);

        for (uint32_t i = 0; i < fftSize; i++)
        {
            dataInTimeDomain[i] = dataInTimeDomain[i] * window.at(i);
        }

        FFTResult result = fftCalculator->calculate(dataInTimeDomain);
        queue.push_back(std::make_unique<FFTResult>(std::move(result)));
        bufforWithDataToBeConverted.erase(bufforWithDataToBeConverted.begin(), bufforWithDataToBeConverted.begin() + numberOfSamplesToBeRemoved);
    }
}

uint32_t WelchCalculator::calculateNumberOfSamplesToBeRemoved()
{
    if(overlapping <= 0)
    {
        return fftSize;
    }

    if(overlapping >= 1)
    {
        const uint32_t atLeastOneSampleMustBeRemoved{1};
        return atLeastOneSampleMustBeRemoved;
    }

    return  (fftSize - (uint32_t)(overlapping * fftSize));
}
