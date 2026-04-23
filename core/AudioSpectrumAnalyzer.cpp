/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioSpectrumAnalyzer.hpp"
#include "dataSource/DataSourceBase.hpp"
#include "Stats.hpp"
#include "Helpers.hpp"
#include "CommonData.hpp"
#include "DataCalculator.hpp"
#include "FrequenciesInfo.hpp"
#include "FftBinCombiner.hpp"
#include <optional>

AudioSpectrumAnalyzer::AudioSpectrumAnalyzer(const Configuration &configuration, std::promise<AppEvent> &&appEvent):
    AudioSpectrumAnalyzerBase(configuration, std::move(appEvent))
{
}

void AudioSpectrumAnalyzer::init()
{
    threads.push_back(std::thread(&AudioSpectrumAnalyzer::samplesUpdater,this));
    threads.push_back(std::thread(&AudioSpectrumAnalyzer::fftCalculator,this));
    threads.push_back(std::thread(&AudioSpectrumAnalyzer::processing,this));
    threads.push_back(std::thread(&AudioSpectrumAnalyzer::drafter,this));
    threads.push_back(std::thread(&AudioSpectrumAnalyzer::flowController,this));
}

void AudioSpectrumAnalyzer::fftCalculator()
{
    const std::string processName{"fftCalculator"};
    StatsManager statsManager(processName);

    float overlapping = calculateOverlapping(config.get<SamplingRate>(), config.get<NumberOfSamples>(), config.get<DesiredFrameRate>());

    WelchCalculator fft(FftType::Real, config.get<NumberOfSamples>(), overlapping,  config.get<SignalWindow>());

    while(shouldProceed)
    {
        const auto &dataInTimeDomain  = dataExchanger.get();
        const auto &newOverlapping = flowControlDataExchanger.getWithoutBlocking();

        if(newOverlapping != std::nullopt)
        {
            overlapping = *newOverlapping;
        }

        if(dataInTimeDomain == nullptr)
        {
            continue;
        }

        statsManager.update();

        auto stereoData = std::any_cast<StereoData>(*dataInTimeDomain);

        fft.updateOverlapping(overlapping);
        fft.updateBuffer(getAverage(stereoData.left, stereoData.right));

        auto fftResult = fft.calculate();

        for(uint32_t i=0; i<fftResult.size(); ++i)
        {
            fftDataExchanger.push_back(std::make_unique<std::any>(std::move(fftResult.at(i))));
        }

    }

    fftDataExchanger.stop();
}

void AudioSpectrumAnalyzer::processing()
{
    const std::string processName{"processing"};
    StatsManager statsManager(processName);

    FrequenciesInfo frequenciesInfo(config.get<SamplingRate>(), config.get<NumberOfSamples>(), config.get<Freqs>());
    DataMaxHolder dataMaxHolder(frequenciesInfo.numberOfFrequencies(), config.get<NumberOfSignalsForMaxHold>(), getFloorDbFs16bit());
    DataAverager dataAverager(frequenciesInfo.numberOfFrequencies(), config.get<NumberOfSignalsForAveraging>());
    DataSmoother dataSmoother(frequenciesInfo.numberOfFrequencies(), config.get<AlphaFactor>());

    FftBinCombiner fftBinCombiner(config.get<ScalingFactor>(), config.get<OffsetFactor>(), frequenciesInfo.getAllFrequencyIndexes());


    while(shouldProceed)
    {
        const auto &fftResult = fftDataExchanger.get();

        if(fftResult == nullptr)
        {
            continue;
        }

        statsManager.update();

        dataMaxHolder.push_back(fftBinCombiner.combineMagnitudes(std::any_cast<FftResult>(*fftResult)));

        auto dataWithMaxValue = dataMaxHolder.calculate();

        if(!dataWithMaxValue.empty())
        {
            dataAverager.push_back(dataWithMaxValue);

            auto averagedData = dataAverager.calculate();

            if(!averagedData.empty())
            {
                dataSmoother.push_back(averagedData);
                auto smoothedData = dataSmoother.calculate();

                processedDataExchanger.push_back(std::make_unique<std::any>(smoothedData));
            }
        }
    }

    processedDataExchanger.stop();
}

AudioSpectrumAnalyzer::~AudioSpectrumAnalyzer()
{
}
