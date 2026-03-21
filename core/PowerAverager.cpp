/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "PowerAverager.hpp"
#include "CommonData.hpp"

PowerAverager::PowerAverager(const float scalingFactor, const float offsetFactor, const FrequencyIndexesPerRectangle &data)
    : scalingFactor(scalingFactor), offsetFactor(offsetFactor), frequencyIndexesPerRectangle(data)
{
}

std::vector<float> PowerAverager::calculate(const std::vector<std::complex<float>>& data)
{
    return linearToDbfs(averagePowerInSpectrum(calculateMagnitude(data)));
}

std::vector<float> PowerAverager::averagePowerInSpectrum(const std::vector<float> &data)
{
    std::vector<float> averagedValues;
    averagedValues.reserve(frequencyIndexesPerRectangle.size());

    for(const auto &[rectangleIndex, frequencyIndexes] : frequencyIndexesPerRectangle)
    {
        const auto count = frequencyIndexes.size();

        switch (count)
        {
            case 0:
                averagedValues.push_back(getFloorDbFs16bit());
                break;

            case 1:
                averagedValues.push_back(data[frequencyIndexes[0]]);
                break;

            default:
            {
                float sum = 0;

                for (const auto& info : frequencyIndexes)
                {
                    sum += data[info];
                }

                averagedValues.push_back(sum / count);
                break;
            }
        }
    }

    return averagedValues;
}

std::vector<float> PowerAverager::linearToDbfs(const std::vector<float> &data)
{
    static constexpr float fullScale16bit = 32767;

    std::vector<float> outputData;
    outputData.reserve(data.size());

    for(const auto &el : data)
    {
        const auto powerInDbfs = 20 * log10(el / fullScale16bit);
        outputData.push_back((powerInDbfs < getFloorDbFs16bit()) ? getFloorDbFs16bit() : powerInDbfs);
    }

    return outputData;
}

std::vector<float> PowerAverager::calculateMagnitude(const std::vector<std::complex<float>> &data)
{
    const uint32_t numberOfSamples = data.size();
    static constexpr float fullScale16bit = 32767;

    std::vector<float> outputData(numberOfSamples);

    for (uint32_t i = 0; i < numberOfSamples; i++)
    {
        const auto magnitude = std::sqrt(data[i].real() * data[i].real() + data[i].imag() * data[i].imag())/((float)numberOfSamples/2);
        outputData[i] = scalingFactor * magnitude + offsetFactor;
    }

    return outputData;
}




