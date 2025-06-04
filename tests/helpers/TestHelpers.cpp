/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "TestHelpers.hpp"

#include <cmath>
#include <random>

float angleInRadiansToDegrees(float radian)
{
   return radian * 180 / M_PI;
}

float angleInDegreesToRadians(float degrees)
{
   return degrees * M_PI / 180;
}

float dbFsToAmplitude(float dBFsPower)
{
    const float fullScale16BitsSignalAmplitude = 32767;
    return fullScale16BitsSignalAmplitude*std::pow(10,(dBFsPower/20));
}

std::vector<float> generateSignal(uint32_t numberOfSamples, float samplingFrequency, float singalFrequency, float amplitude, float angleInDegrees)
{
    const float samplingPeriod = 1.0/samplingFrequency;

    std::vector<float> data(numberOfSamples);

    for(int n=0;n<numberOfSamples;++n)
    {
        data[n]= amplitude * std::sin(2 * M_PI * singalFrequency * n * samplingPeriod + angleInDegreesToRadians(angleInDegrees));
    }

    return data;
}


std::vector<float> addSignals(const std::vector<float> &signal1, const std::vector<float> &signal2)
{
    if(signal1.size()!= signal2.size())
    {
        return {};
    }

    std::vector<float> result(signal1.size());
    std::transform(signal1.begin(), signal1.end(), signal2.begin(), result.begin(), [](const auto &el1, const auto &el2){ return el1+el2;});

    return result;
}

std::vector<float> calculatePhase(const std::vector<std::complex<float>> &signal)
{
    std::vector<float> result(signal.size());
    std::transform(signal.begin(), signal.end(), result.begin(), [](const auto &el){ return angleInRadiansToDegrees(std::arg(el));});

    return result;
}

std::vector<float> generateNoise(uint N, float minValue, float MaxValue)
{

    std::vector<float> data(N);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(minValue,MaxValue);

    for(int i=0;i<N;++i)
    {
        data[i]= dist(rng);
    }

    return data;
}

//function returns Blackman window as we need low side lobes in tests
std::vector<float> getSignalWindow(uint numberOfSamples)
{
    std::vector<float> signalWindow(numberOfSamples);

    for (uint i = 0; i < numberOfSamples; ++i)
    {
        const float ratio = static_cast<float>(i) / (numberOfSamples - 1);

        signalWindow.at(i) = 0.42 - 0.5 * std::cos(2.0 * M_PI * ratio) + 0.08 * std::cos(4.0 * M_PI * ratio);
    }

    return signalWindow;
}
