/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/SamplesCollector.hpp"
#include "helpers/TestHelpers.hpp"
#include "helpers/ValuesChecker.hpp"
#include <gtest/gtest.h>
#include <cmath>
#include <thread>


class SamplesCollectorTests : public ValuesChecker<-6>, public ::testing::Test
{
public:
    const char *pythonFileName = "testAudioConfig";
};

TEST_F(SamplesCollectorTests, samplesCollectorTest)
{
    const uint32_t numberOfSamples{32};
    const float samplingFrequency{32};
    const float singalFrequency{1000};
    const uint32_t amplitude{32767};

    SamplesCollector samplesCollector(pythonFileName);
    samplesCollector.initialize(numberOfSamples,samplingFrequency);
    samplesCollector.collectDataFromHw();

    const auto expectedRightChannel = generateSignal(numberOfSamples, samplingFrequency, singalFrequency, amplitude);
    const auto expectedLeftChannel = generateSignal(numberOfSamples, samplingFrequency, singalFrequency, amplitude, 90);

    auto rightChannel = samplesCollector.getDataFromRightChannel();
    auto leftChannel = samplesCollector.getDataFromLeftChannel();

    valueChecker(rightChannel, expectedRightChannel);
    valueChecker(leftChannel, expectedLeftChannel);
}
