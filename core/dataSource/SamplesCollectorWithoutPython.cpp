/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "SamplesCollector.hpp"
#include "AudioDataSource.hpp"
#include <iostream>

SamplesCollector::SamplesCollector(const bool pythonDataSourceEnabled, const std::string &/*audioConfigFile*/)
{
    dataSourceImpl = std::make_unique<AudioDataSource>();
    if(pythonDataSourceEnabled)
    {
        std::cout<<"This software build does not include Python."<<std::endl;
    }
}

bool SamplesCollector::initialize(uint32_t numberOfSamples, uint32_t sampleRate)
{
    return dataSourceImpl->initialize(numberOfSamples,sampleRate);
}

std::vector<float> SamplesCollector::collectDataFromHw()
{
    return dataSourceImpl->collectDataFromHw();
}

