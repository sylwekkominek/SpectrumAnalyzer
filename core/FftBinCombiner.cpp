/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "FftBinCombiner.hpp"
#include "CommonData.hpp"
#include "Helpers.hpp"

FftBinCombiner::FftBinCombiner(const float scalingFactor, const float offsetFactor, const FrequencyIndexesPerRectangle &data)
    : scalingFactor(scalingFactor), offsetFactor(offsetFactor), frequencyIndexesPerRectangle(data)
{
}

std::vector<float> FftBinCombiner::combineMagnitudes(const std::vector<std::complex<float>>& data)
{
    return linearToDbfs(averageMagnitudeInSpectrum(calculateMagnitude(data)));
}

float FftBinCombiner::combineRmsValues(const std::vector<std::complex<float>>& data)
{
    return linearToDbfs({(float)std::sqrt(getSum(powerInSpectrum(calculateMagnitude(data))))}).at(0);
}

std::vector<float> FftBinCombiner::averageMagnitudeInSpectrum(const std::vector<float> &data)
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

std::vector<double> FftBinCombiner::powerInSpectrum(const std::vector<float> &data)
{
    std::vector<double> result;
    result.reserve(frequencyIndexesPerRectangle.size());

    for(const auto &[rectangleIndex, frequencyIndexes] : frequencyIndexesPerRectangle)
    {
        const auto count = frequencyIndexes.size();

        switch (count)
        {
        case 0:
            result.push_back(getFloorDbFs16bit());
            break;

        case 1:
            result.push_back((data[frequencyIndexes[0]] * data[frequencyIndexes[0]])/2);
            break;

        default:
        {
            double sum = 0;

            for (const auto& info : frequencyIndexes)
            {
                sum += data[info]*data[info];
            }

            result.push_back(sum /2);
            break;
        }
        }
    }

    return result;
}



std::vector<float> FftBinCombiner::linearToDbfs(const std::vector<float> &data)
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

std::vector<float> FftBinCombiner::calculateMagnitude(const std::vector<std::complex<float>> &data)
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




