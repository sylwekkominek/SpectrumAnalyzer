/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "helpers/ValuesChecker.hpp"
#include "helpers/TestHelpers.hpp"
#include "core/FrequenciesInfo.hpp"
#include <gtest/gtest.h>
#include <cmath>


struct FrequenciesInfoParams
{
    uint32_t samplingRate;
    uint32_t fftSize;
    Frequencies demandedFrequencies;
    FrequencyIndexesPerRectangle expectedFrequencyIndexesPerRectangle;
};

class FrequenciesInfoTests : public ValuesChecker<-4>, public ::testing::TestWithParam<FrequenciesInfoParams>
{
public:

    static FrequencyIndexesPerRectangle prepareLinearIndexes(uint32_t samplingRate, uint32_t fftSize, uint32_t stop)
    {
        const uint32_t numberOfRealBins{stop};

        const float minFreq = 1;
        const float maxFreq = 20100;

        const float binWidth = static_cast<float>(samplingRate) / fftSize;

        FrequencyIndexesPerRectangle indexes;


        for (uint32_t i = 1; i < numberOfRealBins; ++i)
        {
            auto freq = i * binWidth;

            if((freq < minFreq) || (freq > maxFreq))
            {
                indexes[i-1] = std::vector<FrequencyIndex>{};
            }
            else
            {
                indexes[i-1] = std::vector<FrequencyIndex>{i};
            }
        }

        return indexes;
    }
};

TEST_P(FrequenciesInfoTests, testingIndexes)
{
    auto params = GetParam();

    FrequenciesInfo frequenciesInfo(params.samplingRate, params.fftSize, params.demandedFrequencies);

    positionValuesChecker<FrequencyIndex>(params.expectedFrequencyIndexesPerRectangle, frequenciesInfo.getAllFrequencyIndexes());
}

TEST_F(FrequenciesInfoTests, testingClosestFrequencies)
{
    uint32_t samplingRate{44100};
    uint32_t fftSize{4096};
    const float binFrequency{(float)samplingRate/fftSize};
    FrequenciesInfo frequenciesInfo(samplingRate, fftSize, {binFrequency, 2*binFrequency, 3*binFrequency, 4*binFrequency, 5*binFrequency});
    valueChecker({0,0,0,1,1,2}, frequenciesInfo.getRectangleIndexesClosestToFrequencies({1.0f, 0.99f * binFrequency, 1.49f * binFrequency, 1.51f * binFrequency, 2.49f * binFrequency, 2.51f * binFrequency}));
}

INSTANTIATE_TEST_SUITE_P(
    FrequenciesInfoTests,
    FrequenciesInfoTests,
    ::testing::Values(
    FrequenciesInfoParams{44100,4096, getDemandedFrequencies(44100, 4096, 1, 1000), FrequenciesInfoTests::prepareLinearIndexes(44100, 4096, 1000)},
    FrequenciesInfoParams{44100,4096, {0,10,20}, FrequencyIndexesPerRectangle{{{0},{}},{{1},{1}},{{2},{2}}}},
    FrequenciesInfoParams{44100,4096, {20080, 20090, 20100, 20110}, FrequencyIndexesPerRectangle{{{0},{1865}},{{1},{1866}},{{2},{}}}},
    FrequenciesInfoParams{44100,4096, {10,50,100},  FrequencyIndexesPerRectangle{{{0},{1,2}},{{1},{3,4,5,6}},{{2},{7,8,9,10,11}}}},
    FrequenciesInfoParams{48000,8192, {10,50,100},  FrequencyIndexesPerRectangle{{{0},{1,2,3,4,5}},{{1},{6,7,8,9,10,11,12}},{{2},{13,14,15,16,17,18,19,20,21}}}}));




