/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Helpers.hpp"
#include "CommonData.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <sstream>

std::vector<float> getAverage(const std::vector<float> &left, const std::vector<float> &right)
{
    if(left.size() != right.size())
    {
        std::cout<<"Vector length mismatch"<<std::endl;
        return {};
    }

    const auto numberOfSamples = right.size();

    std::vector<float> averagedData(numberOfSamples);

    for (uint32_t i = 0; i < numberOfSamples; i++)
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

std::vector<float> calculatePower(const std::vector<std::complex<float>> &fftData, const float scalingFactor, const float offsetFactor)
{
    const uint32_t numberOfSamples = fftData.size();
    static constexpr float fullScale16bit = 32767;

    std::vector<float> outputData(numberOfSamples);

    for (uint32_t i = 0; i < numberOfSamples; i++)
    {
        const auto magnitude = std::sqrt(fftData[i].real() * fftData[i].real() + fftData[i].imag() * fftData[i].imag())/(numberOfSamples/2);
        const auto correctedMagnitude = scalingFactor * magnitude + offsetFactor;
        const auto powerInDbfs = 20*log10(correctedMagnitude/fullScale16bit);

        outputData[i]  = (powerInDbfs < getFloorDbFs16bit()) ? getFloorDbFs16bit() : powerInDbfs;
    }

    return outputData;
}

float calculateOverlappingDiff(const uint32_t desiredNumberOfFramesPerSecond, const uint32_t currentFramesPerSecond)
{
    static constexpr float slope = 0.2;

    auto desiredFps = (desiredNumberOfFramesPerSecond == 0) ? 1 : desiredNumberOfFramesPerSecond;

    return (-slope/desiredFps)*currentFramesPerSecond + slope;
}

float calculateOverlapping(const uint32_t samplingRate, const uint32_t numberOfSamples, const uint32_t numberOfFramesPerSecond)
{
    auto fps = (numberOfFramesPerSecond == 0) ? 1 : numberOfFramesPerSecond;

    return  (1.0 - static_cast<float>(samplingRate)/(numberOfSamples * fps));
}

std::string formatFloat(float value, int totalWidth, int precision)
{
    std::ostringstream oss;

    oss << std::fixed << std::setprecision(precision)
        << std::setw(totalWidth)
        << value;

    return oss.str();
}

std::vector<float> scaleDbfsToPercents(const std::vector<float> &signalInDbfs)
{
    const float hundredPercents = 100;
    const float scallingFactor = hundredPercents/getDynamicRangeOf16bitSignal();

    std::vector<float> outputData(signalInDbfs.size());
    std::transform(signalInDbfs.begin(), signalInDbfs.end(), outputData.begin(), [&scallingFactor](const auto &el){ return el*scallingFactor; });
    return outputData;
}

std::vector<float> moveDbFsToPositiveValues(const std::vector<float> &signalInDbfs)
{
    std::vector<float> outputData(signalInDbfs.size());

    std::transform(signalInDbfs.begin(), signalInDbfs.end(), outputData.begin(), [](const auto &el){ return el+getDynamicRangeOf16bitSignal(); });
    return outputData;
}


