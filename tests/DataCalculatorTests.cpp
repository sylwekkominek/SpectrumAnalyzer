/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/DataCalculator.hpp"
#include "core/Helpers.hpp"
#include "helpers/TestHelpers.hpp"
#include "helpers/ValuesChecker.hpp"
#include <gtest/gtest.h>
#include <cmath>


class DataCalculatorTest: public ValuesChecker<-6>,public ::testing::Test
{
public:
    using Signal = std::vector<float>;
    using Signals = std::vector<Signal>;

    const int firstValueIndex{-2};
    const int lastValueIndex{2};
    const Signals signals{{-1,0,1}, {0,1,2}, {1,2,3}};
};

TEST_F(DataCalculatorTest, dataAveragerTest)
{
    const uint numberOfSamples{8};
    const uint numberOfSignalsForProcessing{3};

    DataAverager dataAverager(numberOfSamples, numberOfSignalsForProcessing);

    for(int i=firstValueIndex;i<=lastValueIndex;++i)
    {
        dataAverager.push_back(Signal(numberOfSamples, i));
    }

    valueChecker(-1, numberOfSamples, dataAverager.calculateWithMoving());
    valueChecker(0, numberOfSamples, dataAverager.calculateWithMoving());
    valueChecker(1, numberOfSamples, dataAverager.calculateWithMoving());
    valueChecker(2, 0, dataAverager.calculateWithMoving());
}

TEST_F(DataCalculatorTest, dataAveragerTest2)
{
    const uint numberOfSamples{3};
    const uint numberOfSignalsForProcessing{3};

    DataAverager dataAverager(numberOfSamples, numberOfSignalsForProcessing);

    for(const auto &signal: signals)
    {
        dataAverager.push_back(signal);
    }

    const auto signal = dataAverager.calculateWithMoving();

    valueChecker(signal, {0,1,2});
}

TEST_F(DataCalculatorTest, dataMaxHolderTest)
{
    const uint numberOfSamples{8};
    const uint numberOfSignalsForProcessing{3};

    DataMaxHolder dataMaxHolder(numberOfSamples, numberOfSignalsForProcessing);

    for(int i=firstValueIndex;i<=lastValueIndex;++i)
    {
        dataMaxHolder.push_back(Signal(numberOfSamples, i));
    }

    dataMaxHolder.calculateWithMoving();
    valueChecker(1, numberOfSamples, dataMaxHolder.calculateWithMoving());
    valueChecker(2, numberOfSamples, dataMaxHolder.calculateWithMoving());
    valueChecker(3, 0, dataMaxHolder.calculateWithMoving());
}

TEST_F(DataCalculatorTest, dataMaxHolderTest2)
{
    const uint numberOfSamples{3};
    const uint numberOfSignalsForProcessing{3};

    DataMaxHolder dataMaxHolder(numberOfSamples, numberOfSignalsForProcessing);

    for(const auto &signal: signals)
    {
        dataMaxHolder.push_back(signal);
    }

    const auto signal = dataMaxHolder.calculateWithMoving();

    valueChecker(signal, {1,2,3});
}
