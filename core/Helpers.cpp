/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Helpers.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cstdint>

std::vector<float> getAverage(const std::vector<float> &left, const std::vector<float> &right)
{
    if(left.size() != right.size())
    {
        std::cout<<"Vector length mismatch"<<std::endl;
        return {};
    }

    const auto numberOfSamples = right.size();

    std::vector<float> averagedData(numberOfSamples);

    for (uint i = 0; i < numberOfSamples; i++)
    {
        averagedData[i] = (left[i] + right[i])/2.0;
    }

    return averagedData;
}

void zoomData(std::vector<float> &data, const float factor, const float offset)
{
    for(auto &el:data)
    {
        el = factor*(el-offset);
    }
}

std::vector<float> normalize(const std::vector<float> &fftData)
{
    const uint32_t numberOfSamples = fftData.size();
    std::vector<float> outputData(numberOfSamples);

    std::transform(fftData.begin(), fftData.end(), outputData.begin(), [&numberOfSamples](const auto &el){ return (el/(numberOfSamples/2)); });
    return outputData;
}

std::vector<float> calculatePower(const std::vector<std::complex<float>> &fftData)
{

    const uint32_t numberOfSamples = fftData.size();

    std::vector<float> outputData(numberOfSamples);

    for (uint i = 0; i < numberOfSamples; i++)
    {
        auto data =  std::sqrt(fftData[i].real() * fftData[i].real() + fftData[i].imag() * fftData[i].imag())/(numberOfSamples/2);
        outputData[i]  = (data >=1) ? 20*log10(data) : 0;
    }

    return outputData;
}

