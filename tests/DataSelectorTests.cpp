/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "helpers/ValuesChecker.hpp"
#include "core/DataSelector.hpp"
#include <gtest/gtest.h>
#include <cmath>

struct InputTestData
{
    uint32_t fftSize;
    uint32_t samplingRate;
    Frequencies frequencies;
    std::vector<uint32_t> expectedIndexes;
};

class DataSelectorTests : public ValuesChecker<-4>, public ::testing::TestWithParam<InputTestData>
{
public:
    const uint32_t samplingRate{44100};

    std::vector<float> prepareFrequencyDomainData(const uint32_t fftSize, const std::vector<uint32_t> &expectedIndexes)
    {
        const auto uniqueIndexes = std::set(expectedIndexes.begin(), expectedIndexes.end());

        std::vector<float> fftData(fftSize);

        uint32_t i=1;

        for(auto &index : uniqueIndexes)
        {
            fftData.at(index) = i++;
        }

        return fftData;
    }

    std::vector<float> prepareExpectedData(const std::vector<uint32_t> &expectedIndexes, const std::vector<float> &frequencyDomainData)
    {
        std::vector<float> data;

        data.reserve(expectedIndexes.size());

        for(auto &index : expectedIndexes)
        {
            data.push_back(frequencyDomainData.at(index));
        }

        return data;
    }

};

TEST_P(DataSelectorTests, testDataSelector)
{
    const auto& param = GetParam();

    DataSelector dataSelector(param.samplingRate, param.fftSize, param.frequencies);
    auto frequencyDomainData = prepareFrequencyDomainData(param.fftSize, param.expectedIndexes);
    auto expectedData = prepareExpectedData(param.expectedIndexes, frequencyDomainData);

    valueChecker(expectedData, dataSelector(frequencyDomainData));
}

INSTANTIATE_TEST_SUITE_P(
    DataSelectorTests,
    DataSelectorTests,
    ::testing::Values(
        InputTestData{1024, 44100, {42,43,44,1012,1013,5000}, {1,1,1,23,24,116}},
        InputTestData{2048, 44100, {20,21,22,1001,1002,5000}, {1,1,1,46,47,232}},
        InputTestData{4096, 44100, {10,11,12,995,996,5000},   {1,1,1,92,93,464}},
        InputTestData{4096, 48000, {10,11,12,1001,1002,5000}, {1,1,1,85,86,427}})
    );


class DataSelectorTests2 : public ValuesChecker<-4>, public ::testing::Test
{
};


TEST_F(DataSelectorTests2, testDataSelector)
{
    const std::vector<float> expectedIndexes{0,1,1,2,3,3,4,4};
    DataSelector dataSelector(44100, 4096, {20,100,360,1000,2000,3000});
    valueChecker(expectedIndexes, dataSelector.getRectangleIndexesClosestToFrequencies({25,70, 150, 250, 700, 1400, 1600,2200}));
}
