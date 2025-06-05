/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "FFTCalculator.hpp"
#include "DataExchanger.hpp"

FFTCalculator::FFTCalculator(uint32_t size): inPtr(std::make_unique<std::vector<fftw_complex>>(size)), outPtr(std::make_unique<std::vector<fftw_complex>>(size))
{
    p = fftw_plan_dft_1d(size, inPtr->data(), outPtr->data(), FFTW_FORWARD,  FFTW_ESTIMATE);
}

FFTResult FFTCalculator::calculate(const std::vector<float> &inputData)
{

    for(uint i =0; i<inPtr->size(); ++i)
    {
        (*inPtr)[i][0] = inputData.at(i);
        (*inPtr)[i][1] = 0;
    }


    fftw_execute(p);

    std::vector<std::complex<float>> outputData;
    outputData.reserve(outPtr->size());

    for(uint i =0; i<outPtr->size(); ++i)
    {
        outputData.emplace_back((*outPtr)[i][0],(*outPtr)[i][1]);
    }

    return outputData;
}


FFTCalculator::~FFTCalculator()
{
    fftw_destroy_plan(p);
    fftw_cleanup();
}

WelchCalculator::WelchCalculator(const uint32_t fftSize, const float overlapping, const std::vector<float> window) :
    FFTCalculator(fftSize),
    overlapping(overlapping),
    numberOfSamplesToBeRemoved(calculateNumberOfSamplesToBeRemoved()),
    fftSize(fftSize), window(window)
{
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

        for (uint i = 0; i < fftSize; i++)
        {
            dataInTimeDomain[i] = dataInTimeDomain[i] * window.at(i);
        }

        FFTResult result = FFTCalculator::calculate(dataInTimeDomain);
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
