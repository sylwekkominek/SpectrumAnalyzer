/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/IndexSelector.hpp"
#include <gtest/gtest.h>
#include <cmath>

class IndexSelectorTests : public ::testing::Test
{
public:
    const uint samplingRate{44100};

    void checkFrequencyIndex(const std::vector<uint> &expectedIndexes, const IndexSelector &indexSelector)
    {
        for(int i=0;i< expectedIndexes.size(); ++i)
        {
            EXPECT_EQ(expectedIndexes.at(i), indexSelector.getFrequencyIndex(i));
        }
    }
};

TEST_F(IndexSelectorTests, testIndexesWithFFTSize1024)
{
    const uint fftSize{1024};
    const Frequencies frequencies{42,43,44,1012,1013,5000};
    const std::vector<uint> expectedIndexes{1,1,1,23,24,116};

    IndexSelector indexSelector(samplingRate, fftSize, frequencies);
    checkFrequencyIndex(expectedIndexes, indexSelector);
}

TEST_F(IndexSelectorTests, testIndexesWithFFTSize2048)
{
    const uint fftSize{2048};
    const Frequencies frequencies{20,21,22,1001,1002,5000};
    const std::vector<uint> expectedIndexes{1,1,1,46,47,232};

    IndexSelector indexSelector(samplingRate, fftSize, frequencies);
    checkFrequencyIndex(expectedIndexes, indexSelector);
}

TEST_F(IndexSelectorTests, testIndexesWithFFTSize4096)
{
    const uint fftSize{4096};
    const Frequencies frequencies{10,11,12,995,996,5000};
    const std::vector<uint> expectedIndexes{1,1,1,92,93,464};

    IndexSelector indexSelector(samplingRate, fftSize, frequencies);
    checkFrequencyIndex(expectedIndexes, indexSelector);
}

TEST_F(IndexSelectorTests, testIndexesWithSamplingRate48000)
{
    const uint fftSize{4096};
    const uint samplingRate{48000};

    const Frequencies frequencies{10,11,12,1001,1002,5000};
    const std::vector<uint> expectedIndexes{1,1,1,85,86,427};

    IndexSelector indexSelector(samplingRate, fftSize, frequencies);
    checkFrequencyIndex(expectedIndexes, indexSelector);
}
