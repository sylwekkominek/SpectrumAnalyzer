/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "helpers/ValuesChecker.hpp"
#include "core/PowerAverager.hpp"
#include <gtest/gtest.h>
#include <cmath>

struct PowerAveragerParams
{
    size_t fftSize;
    std::vector<std::pair<uint16_t, float>> binsWithMagnitudes;
    std::vector<float> expectedDbfsValues;
    FrequencyIndexesPerRectangle frequencyIndexes;
};

class PowerAveragerTests : public ValuesChecker<-3>, public ::testing::TestWithParam<PowerAveragerParams>
{
public:

    static std::vector<std::complex<float>> createFakeFft(uint16_t fftSize, const std::vector<std::pair<uint16_t, float>>& binsWithMagnitudes)
    {
        std::vector<std::complex<float>> data(fftSize, {0.0f, 0.0f});

        for (const auto &[binIndex, magnitude] : binsWithMagnitudes)
        {
            data.at(binIndex) = {magnitude * fftSize/2, 0.0f};
        }

        return data;
    }
};

TEST_P(PowerAveragerTests, test)
{
    auto params = GetParam();

    PowerAverager powerAverager(1,0, params.frequencyIndexes);
    std::vector<std::complex<float>> data = createFakeFft(params.fftSize, params.binsWithMagnitudes);
    valueChecker(params.expectedDbfsValues, powerAverager.calculate(data));
}

INSTANTIATE_TEST_SUITE_P(
    PowerAveragerTests,
    PowerAveragerTests,
    ::testing::Values(
        PowerAveragerParams{4096,{{0,32767}, {1,32767}, {2,32767}},{0.0, 0.0, 0.0},FrequencyIndexesPerRectangle{{{0},{0}},{{1},{1}},{{2},{2}}}},
        PowerAveragerParams{4096,{{0,16384}, {1, 16384}, {2, 32767}, {3, 32767}},{-6.02f, 0.0f, 0.0f},FrequencyIndexesPerRectangle{{{0},{0,1}},{{1},{2}},{{2},{3}}}},
        PowerAveragerParams{4096,{{0,1}, {1, 1}, {2, 1}, {3, 8}, {4, 8}, {5, 16}},{-90.308f, -72.247f, -66.226f},FrequencyIndexesPerRectangle{{{0},{0,1,2}},{{1},{3,4}},{{2},{5}}}})
    );
