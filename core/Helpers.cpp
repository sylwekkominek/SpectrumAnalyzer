/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Helpers.hpp"
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <numeric>


double getSum(const std::vector<double> &data)
{
    if (data.empty())
    {
        return 0.0;
    }

    return std::accumulate(data.begin(), data.end(), 0.0);
}

float getAverage(const std::vector<float> &data)
{
    if (data.empty())
    {
        return 0.0f;
    }

    const float sum = std::accumulate(data.begin(), data.end(), 0.0f);
    return sum / data.size();
}

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

// dBFS to Percent mapping:
//
// startDbFs (top = 100% of the screen)
//        |
//        |
//        |
// stopDbFs (bottom = 0% of the screen)

std::vector<float> scaleDbfsToPercents(const std::vector<float> &dataInDbfs, float startDbFs, float stopDbFs)
{
    const float hundredPercents = 100;
    const float scallingFactor = hundredPercents/(startDbFs - stopDbFs);
    const float offsetFactor = -stopDbFs;

    std::vector<float> outputData(dataInDbfs.size());
    std::transform(dataInDbfs.begin(), dataInDbfs.end(), outputData.begin(), [&scallingFactor, &offsetFactor](const auto &el){ return (el+offsetFactor)*scallingFactor; });

    return outputData;
}





