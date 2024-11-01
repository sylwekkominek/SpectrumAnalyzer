/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "DataCalculator.hpp"


DataAverager::DataAverager(uint numberOfSamples, uint numberOfSignalsForProcessing):
    DataCalculatorBase<float>(numberOfSamples, numberOfSignalsForProcessing)

{
    updateAlgorithm();
}

void DataAverager::updateAlgorithm()
{
    algorithm = [this](auto left, auto right)
    {
        return left + right * (1.0/numberOfSignalsForProcessing);
    };
}

DataMaxHolder::DataMaxHolder(uint numberOfSamples, uint numberOfSignalsForProcessing):
    DataCalculatorBase(numberOfSamples, numberOfSignalsForProcessing)

{
    updateAlgorithm();
}


void DataMaxHolder::updateAlgorithm()
{
    algorithm = [this](auto left, auto right)
    {
        return std::max(left,right);
    };
}
