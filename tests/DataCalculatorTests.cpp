/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/DataCalculator.hpp"
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
    const uint32_t numberOfSamples{8};
    const uint32_t numberOfSignalsForProcessing{3};

    DataAverager dataAverager(numberOfSamples, numberOfSignalsForProcessing);

    for(int i=firstValueIndex;i<=lastValueIndex;++i)
    {
        dataAverager.push_back(Signal(numberOfSamples, i));
    }

    valueChecker(-1, numberOfSamples, dataAverager.calculate());
    valueChecker(0, numberOfSamples, dataAverager.calculate());
    valueChecker(1, numberOfSamples, dataAverager.calculate());
    valueChecker(2, 0, dataAverager.calculate());
}

TEST_F(DataCalculatorTest, dataAveragerTest2)
{
    const uint32_t numberOfSamples{3};
    const uint32_t numberOfSignalsForProcessing{3};

    DataAverager dataAverager(numberOfSamples, numberOfSignalsForProcessing);

    for(const auto &signal: signals)
    {
        dataAverager.push_back(signal);
    }

    const auto signal = dataAverager.calculate();

    valueChecker(signal, {0,1,2});
}

TEST_F(DataCalculatorTest, dataMaxHolderTest)
{
    const uint32_t numberOfSamples{8};
    const uint32_t numberOfSignalsForProcessing{3};

    DataMaxHolder dataMaxHolder(numberOfSamples, numberOfSignalsForProcessing);

    for(int i=firstValueIndex;i<=lastValueIndex;++i)
    {
        dataMaxHolder.push_back(Signal(numberOfSamples, i));
    }

    valueChecker(0, numberOfSamples, dataMaxHolder.calculate());
    valueChecker(1, numberOfSamples, dataMaxHolder.calculate());
    valueChecker(2, numberOfSamples, dataMaxHolder.calculate());
    valueChecker(3, 0, dataMaxHolder.calculate());
}

TEST_F(DataCalculatorTest, dataMaxHolderTest2)
{
    const uint32_t numberOfSamples{3};
    const uint32_t numberOfSignalsForProcessing{3};

    DataMaxHolder dataMaxHolder(numberOfSamples, numberOfSignalsForProcessing);

    for(const auto &signal: signals)
    {
        dataMaxHolder.push_back(signal);
    }

    const auto signal = dataMaxHolder.calculate();

    valueChecker(signal, {1,2,3});
}

TEST_F(DataCalculatorTest, dataSmootherTest)
{
    const uint32_t numberOfSamples{3};
    const float alphaFactor{0.1};

    DataSmoother dataSmoother(numberOfSamples, alphaFactor);

    for(int i=firstValueIndex;i<=lastValueIndex;++i)
    {
        dataSmoother.push_back(Signal(numberOfSamples, i));
    }

    valueChecker(-0.2, numberOfSamples, dataSmoother.calculate());
    valueChecker(-0.28, numberOfSamples, dataSmoother.calculate());
    valueChecker(-0.252, numberOfSamples, dataSmoother.calculate());
    valueChecker(-0.1268, numberOfSamples, dataSmoother.calculate());
    valueChecker(0.08588, numberOfSamples, dataSmoother.calculate());
}

TEST_F(DataCalculatorTest, dataSmootherTest2)
{

    const uint32_t numberOfSamples{3};
    const float alphaFactor{0.1};

    DataSmoother dataSmoother(numberOfSamples, alphaFactor);

    for(const auto &signal: signals)
    {
        dataSmoother.push_back(signal);
    }

    const auto signal = dataSmoother.calculate();

    valueChecker(signal, {-0.1, 0,0.1});
}
