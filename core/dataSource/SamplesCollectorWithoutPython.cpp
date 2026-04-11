/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "SamplesCollector.hpp"
#include "AudioDataSource.hpp"
#include <iostream>

SamplesCollector::SamplesCollector(const bool pythonDataSourceEnabled, bool loopbackEnabled, const std::string &/*audioConfigFile*/)
{
    dataSourceImpl = std::make_unique<AudioDataSource>(loopbackEnabled);
    if(pythonDataSourceEnabled)
    {
        std::cout<<"This software build does not include Python."<<std::endl;
    }
}

bool SamplesCollector::initialize(uint32_t numberOfSamples, uint32_t sampleRate)
{
    return dataSourceImpl->initialize(numberOfSamples,sampleRate);
}

StereoData SamplesCollector::collectStereoDataFromHw()
{
    return dataSourceImpl->collectStereoDataFromHw();
}

bool SamplesCollector::checkIfErrorOccured()
{
    return dataSourceImpl->checkIfErrorOccured();
}
