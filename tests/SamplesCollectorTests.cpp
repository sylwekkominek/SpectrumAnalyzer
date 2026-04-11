/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/dataSource/PythonDataSource.hpp"
#include "helpers/TestHelpers.hpp"
#include "helpers/ValuesChecker.hpp"
#include <gtest/gtest.h>
#include <cmath>


class PythonDataSourceTests : public ValuesChecker<-1,2>, public ::testing::Test
{
public:
    const char *pythonFileName = "testAudioConfig";
};

TEST_F(PythonDataSourceTests, pythonDataSourceTest)
{
    const uint32_t numberOfSamples{1024};
    const float samplingFrequency{44100};

    const uint32_t leftSignalAmplitude = 32767;
    const uint32_t rightSignalAmplitude = 16384;

    const auto expectedLeftSignal = generateSignal(numberOfSamples, samplingFrequency, 1000, leftSignalAmplitude);
    const auto expectedRightSignal = generateSignal(numberOfSamples, samplingFrequency, 1000, rightSignalAmplitude);

    PythonDataSource pythonDataSource(pythonFileName);
    pythonDataSource.initialize(numberOfSamples,samplingFrequency);
    auto stereoData = pythonDataSource.collectStereoDataFromHw();

    valueChecker(expectedLeftSignal, stereoData.left);
    valueChecker(expectedRightSignal, stereoData.right);
}
