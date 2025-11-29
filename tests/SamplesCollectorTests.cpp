/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/dataSource/PythonDataSource.hpp"
#include "helpers/TestHelpers.hpp"
#include "helpers/ValuesChecker.hpp"
#include <gtest/gtest.h>
#include <cmath>


class PythonDataSourceTests : public ValuesChecker<-6>, public ::testing::Test
{
public:
    const char *pythonFileName = "testAudioConfig";
};

TEST_F(PythonDataSourceTests, pythonDataSourceTest)
{
    const uint32_t numberOfSamples{32};
    const float samplingFrequency{32};
    const float singalFrequency{1000};
    const uint32_t amplitude{32767};

    const auto expectedSignal = generateSignal(numberOfSamples, samplingFrequency, singalFrequency, amplitude);

    PythonDataSource pythonDataSource(pythonFileName);
    pythonDataSource.initialize(numberOfSamples,samplingFrequency);
    pythonDataSource.collectDataFromHw();

    valueChecker(expectedSignal, pythonDataSource.collectDataFromHw());
}
