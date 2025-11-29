/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioSpectrumAnalyzer.hpp"
#include "dataSource/SamplesCollector.hpp"
#include "ConfigReader.hpp"
#include "Window.hpp"
#include "Stats.hpp"
#include "Helpers.hpp"
#include "CommonData.hpp"
#include "DataCalculator.hpp"
#include <iostream>
#include <optional>

AudioSpectrumAnalyzer::AudioSpectrumAnalyzer(const Configuration &configuration):
    SpectrumAnalyzerBase(configuration)
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

void AudioSpectrumAnalyzer::samplesUpdater()
{
    const uint32_t noOfSamplesToBeCollectedFromHwEachTime{128};
    const std::string processName{"samplesUpdater"};
    StatsManager statsManager(processName);

    SamplesCollector samplesCollector(config.get<PythonDataSourceEnabled>(), audioConfigFile);

    if(samplesCollector.initialize(noOfSamplesToBeCollectedFromHwEachTime, config.get<SamplingRate>())==false)
    {
        shouldProceed = false;
    }

    while(shouldProceed)
    {
        statsManager.update();
        dataExchanger.push_back(std::make_unique<Data>(std::move(samplesCollector.collectDataFromHw())));
    }

    dataExchanger.stop();
}

void AudioSpectrumAnalyzer::fftCalculator()
{
    const std::string processName{"fftCalculator"};
    StatsManager statsManager(processName);

    float overlapping = calculateOverlapping(config.get<SamplingRate>(), config.get<NumberOfSamples>(), config.get<DesiredFrameRate>());

    WelchCalculator fft(config.get<NumberOfSamples>(), overlapping,  config.get<SignalWindow>());

    using namespace std::chrono;

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

        fft.updateOverlapping(overlapping);
        fft.updateBuffer(*dataInTimeDomain);
        fft.calculate(fftDataExchanger);
    }

    fftDataExchanger.stop();
}


void AudioSpectrumAnalyzer::processing()
{
    const std::string processName{"processing"};
    StatsManager statsManager(processName);
    std::deque<Data> fftDataQueue;
    std::deque<Data> averagedDataQueue;

    DataMaxHolder dataMaxHolder(config.get<NumberOfSamples>(), config.get<NumberOfSignalsForMaxHold>(), getFloorDbFs16bit());
    DataAverager dataAverager(config.get<NumberOfSamples>(), config.get<NumberOfSignalsForAveraging>());
    DataSmoother dataSmoother(config.get<NumberOfSamples>(), config.get<AlphaFactor>());


    while(shouldProceed)
    {
        const auto &fftResult = fftDataExchanger.get();

        if(fftResult == nullptr)
        {
            continue;
        }

        statsManager.update();

        auto power = calculatePower(*fftResult, config.get<ScalingFactor>(), config.get<OffsetFactor>());

        dataMaxHolder.push_back(power);
        auto dataWithMaxValue = dataMaxHolder.calculate();

        if(not dataWithMaxValue.empty())
        {
            dataAverager.push_back(dataWithMaxValue);

            auto averagedData = dataAverager.calculate();

            if(not averagedData.empty())
            {
                dataSmoother.push_back(averagedData);
                auto smoothedData = dataSmoother.calculate();

                processedDataExchanger.push_back(std::make_unique<Data>(smoothedData));
            }
        }
    }

    processedDataExchanger.stop();
}

void AudioSpectrumAnalyzer::drafter()
{
    const std::string processName{"drafter"};
    StatsManager statsManager(processName);

    bool isFullScreenEnabled = config.get<DefaultFullscreenState>();

    std::unique_ptr<Window> window = std::make_unique<Window>(config, isFullScreenEnabled);
    window->initializeGPU();

    while(shouldProceed)
    {
        const auto &data = processedDataExchanger.get();

        if(data == nullptr)
        {
            continue;
        }

        statsManager.update();

        window->draw(*data);

        if(window->checkIfWindowShouldBeClosed())
        {
            shouldProceed.store(false);
        }

        if(window->checkIfWindowShouldBeRecreated())
        {
            isFullScreenEnabled = !isFullScreenEnabled;
            window = std::make_unique<Window>(config, isFullScreenEnabled);
            window->initializeGPU();
        }
    }

}
void AudioSpectrumAnalyzer::flowController()
{
    float coeffUsedInCaseWhenScreenFallsBehindIncomingData = -0.01;

    auto previousTime = steady_clock::now();

    while(shouldProceed)
    {
        std::this_thread::sleep_for(100ms);

        auto numberOfFramesPerSecond = StatsManager::getStatsFor("drafter").getNumberOfCallsInLast(1000ms);
        auto overlappingDiff = calculateOverlappingDiff(config.get<DesiredFrameRate>(), numberOfFramesPerSecond);
        auto overlapping = calculateOverlapping(config.get<SamplingRate>(), config.get<NumberOfSamples>(), numberOfFramesPerSecond);

        if(processedDataExchanger.getSize() > 1)
        {
            overlapping = overlapping + coeffUsedInCaseWhenScreenFallsBehindIncomingData;
        }

        overlapping = overlapping + overlappingDiff;

        if((overlapping >=0) && (overlapping < 1))
        {
            flowControlDataExchanger.push_back(std::move(overlapping));
        }

        auto now = steady_clock::now();

        if(now - previousTime >= seconds(1))
        {
            std::cout<<"Samples are updated: "<<StatsManager::getStatsFor("samplesUpdater").getNumberOfCallsInLast(1000ms)<<" per second"<< " queue size: "<<dataExchanger.getSize()<<std::endl;
            std::cout<<"Plots are updated: "<<numberOfFramesPerSecond<<" per second"<<" queue size: "<<processedDataExchanger.getSize()<<std::endl;
            previousTime = now;
        }
    }
}

AudioSpectrumAnalyzer::~AudioSpectrumAnalyzer()
{
}
