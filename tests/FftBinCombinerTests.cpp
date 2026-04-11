/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "helpers/ValuesChecker.hpp"
#include "helpers/TestHelpers.hpp"
#include "core/FftBinCombiner.hpp"
#include <gtest/gtest.h>
#include <cmath>

namespace
{
std::vector<FrequencyIndex> getAllFrequencyIndexes(uint16_t fftSize)
{
    uint16_t numberOfBins = fftSize/2;

    std::vector<FrequencyIndex> result;
    result.resize(numberOfBins);

    for(uint16_t i=0; i<numberOfBins; ++i)
    {
        result.push_back(i);
    }

    return result;
}
}


struct FftBinCombinerParams
{
    size_t fftSize;
    std::vector<std::pair<uint16_t, float>> binsWithMagnitudes;
    std::vector<float> expectedDbfsValues;
    FrequencyIndexesPerRectangle frequencyIndexes;
};

class FftBinCombinerTests : public ValuesChecker<-3>, public ::testing::TestWithParam<FftBinCombinerParams>
{
};

TEST_P(FftBinCombinerTests, test)
{
    auto params = GetParam();

    FftBinCombiner fftBinCombiner(1,0, params.frequencyIndexes);
    std::vector<std::complex<float>> data = createFakeFft(params.fftSize, params.binsWithMagnitudes);
    valueChecker(params.expectedDbfsValues, fftBinCombiner.combineMagnitudes(data));
}

INSTANTIATE_TEST_SUITE_P(
    FftBinCombinerTests,
    FftBinCombinerTests,
    ::testing::Values(
        FftBinCombinerParams{4096,{{0,32767}, {1,32767}, {2,32767}},{0.0, 0.0, 0.0}, FrequencyIndexesPerRectangle{{{0},{0}},{{1},{1}},{{2},{2}}}},
        FftBinCombinerParams{4096,{{0,16384}, {1, 16384}, {2, 32767}, {3, 32767}},{-6.02f, 0.0f, 0.0f},  FrequencyIndexesPerRectangle{{{0},{0,1}},{{1},{2}},{{2},{3}}}},
        FftBinCombinerParams{4096,{{0,1}, {1, 1}, {2, 1}, {3, 8}, {4, 8}, {5, 16}},{-90.308f, -72.247f, -66.226f}, FrequencyIndexesPerRectangle{{{0},{0,1,2}},{{1},{3,4}},{{2},{5}}}})
    );

struct FftBinCombinerRmsParams
{
    size_t fftSize;
    std::vector<std::pair<uint16_t, float>> binsWithMagnitudes;
    float expectedDbfsValue;
    FrequencyIndexesPerRectangle frequencyIndexes;
};

class FftBinCombinerTests2 : public ValuesChecker<-3>, public ::testing::TestWithParam<FftBinCombinerRmsParams>
{
};

TEST_P(FftBinCombinerTests2, test)
{
    auto params = GetParam();

    FftBinCombiner fftBinCombiner(1,0, params.frequencyIndexes);
    std::vector<std::complex<float>> data = createFakeFft(params.fftSize, params.binsWithMagnitudes);
    EXPECT_NEAR(params.expectedDbfsValue ,fftBinCombiner.combineRmsValues(data),marginOfError);
}

INSTANTIATE_TEST_SUITE_P(
    FftBinCombinerTests2,
    FftBinCombinerTests2,
    ::testing::Values(
        FftBinCombinerRmsParams{4096,{{100,32767}},-3.0103,  FrequencyIndexesPerRectangle{{{0, {99,100,101}}}}},
        FftBinCombinerRmsParams{4096,{{99,8192}, {100,32767},{101,16384}},-3.0103, FrequencyIndexesPerRectangle{{{0, {100}}}}},
        FftBinCombinerRmsParams{4096,{{99,32767}, {100,32767},{101,32767}},1.76091, FrequencyIndexesPerRectangle{{{0, {99,100,101}}}}}
        )
    );
