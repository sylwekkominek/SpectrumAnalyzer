/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioSpectrumAnalyzerBase.hpp"
#include "Stats.hpp"
#include "dataSource/SamplesCollector.hpp"
#include "CommonData.hpp"
#include "Helpers.hpp"
#include "Window.hpp"
#include <iostream>


void AudioSpectrumAnalyzerBase::samplesUpdater()
{
    const uint32_t noOfSamplesToBeCollectedFromHwEachTime{128};
    const std::string processName{"samplesUpdater"};
    StatsManager statsManager(processName);

    SamplesCollector samplesCollector(config.get<PythonDataSourceEnabled>(), config.get<LoopbackEnabled>(), audioConfigFile);

    samplesCollector.initialize(noOfSamplesToBeCollectedFromHwEachTime, config.get<SamplingRate>());

    auto channel = std::vector<float>(config.get<NumberOfSamples>(),getFloorDbFs16bit());

    while(shouldProceed)
    {
        statsManager.update();

        if(samplesCollector.checkIfErrorOccured())
        {
            for(int i=0;i< config.get<DesiredFrameRate>();++i)
            {
                dataExchanger.push_back(std::make_unique<std::any>(StereoData{channel, channel}));
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / config.get<DesiredFrameRate>()));
            }

            std::cout<<"PLEASE ENABLE YOUR MICROPHONE OR ANOTHER AUDIO INPUT DEVICE."<<std::endl;

            samplesCollector.initialize(noOfSamplesToBeCollectedFromHwEachTime, config.get<SamplingRate>());
        }
        else
        {
            auto data = samplesCollector.collectStereoDataFromHw();

            if(!data.left.empty() and !data.right.empty())
            {
                dataExchanger.push_back(std::make_unique<std::any>(std::move(data)));
            }
            else
            {
                dataExchanger.push_back(std::make_unique<std::any>(StereoData{channel, channel}));
            }
        }
    }

    dataExchanger.stop();
}

void AudioSpectrumAnalyzerBase::drafter()
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

        window->draw(std::any_cast<Data>(*data));


        if(window->checkIfWindowShouldBeRecreated())
        {
            isFullScreenEnabled = !isFullScreenEnabled;
            window = std::make_unique<Window>(config, isFullScreenEnabled);
            window->initializeGPU();
        }

        if(window->checkIfWindowShouldBeClosed())
        {
            appEventPromise.set_value(ApplicationState::Shutdown);
            shouldProceed.store(false);
        }

        if (auto themeConfig = window->checkIfThemeShouldBeChanged())
        {
            appEventPromise.set_value(*themeConfig);
            shouldProceed.store(false);
        }
    }
}

void AudioSpectrumAnalyzerBase::flowController()
{
    float coeffUsedInCaseWhenScreenFallsBehindIncomingData = -0.01;

    auto previousTime = steady_clock::now();

    // wait for 2 seconds to prevent overlapping updates
    while (shouldProceed)
    {
        if (std::chrono::steady_clock::now() - previousTime >= 2s)
            break;

        std::this_thread::sleep_for(100ms);
    }

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

