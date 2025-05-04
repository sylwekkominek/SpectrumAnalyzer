/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "DataCalculator.hpp"


DataAverager::DataAverager(uint numberOfSamples, uint numberOfSignalsForProcessing):
    DataCalculatorBase<float>(numberOfSamples, numberOfSignalsForProcessing)

{
    updateAlgorithm();
}

std::vector<float> DataAverager::calculate()
{
    return calculateWithMoving();
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

std::vector<float> DataMaxHolder::calculate()
{
    return calculateWithMoving();
}

void DataMaxHolder::updateAlgorithm()
{
    algorithm = [this](auto left, auto right)
    {
        return std::max(left,right);
    };
}

DataSmoother::DataSmoother(uint numberOfSamples, float alphaFactor):
    DataCalculatorBase(numberOfSamples, 1), alphaFactor(alphaFactor)

{
    updateAlgorithm();
}

std::vector<float> DataSmoother::calculate()
{
    return calculateWithStoring();
}

void DataSmoother::updateAlgorithm()
{
    algorithm = [this](auto left, auto right)
    {
        return (1.0-alphaFactor)*left + alphaFactor*right;
    };
}
