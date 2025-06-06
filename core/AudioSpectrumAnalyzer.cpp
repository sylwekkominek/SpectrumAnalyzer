/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioSpectrumAnalyzer.hpp"
#include "SamplesCollector.hpp"
#include "ConfigReader.hpp"
#include "Window.hpp"
#include "IndexSelector.hpp"
#include "Stats.hpp"
#include "Helpers.hpp"
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

    SamplesCollector samplesCollector(audioConfigFile.c_str());
    samplesCollector.initialize(noOfSamplesToBeCollectedFromHwEachTime, config.samplingRate);

    while(shouldProceed)
    {
        statsManager.update();

        samplesCollector.collectDataFromHw();

        auto rightdata = samplesCollector.getDataFromRightChannel();
        auto leftdata  = samplesCollector.getDataFromLeftChannel();

        dataExchanger.push_back(std::make_unique<Data>(std::move(getAverage(leftdata,rightdata))));
    }

    dataExchanger.stop();
}

void AudioSpectrumAnalyzer::fftCalculator()
{
    const std::string processName{"fftCalculator"};
    StatsManager statsManager(processName);

    float overlapping = calculateOverlapping(config.samplingRate, config.numberOfSamples, config.desiredFrameRate);

    WelchCalculator fft(config.numberOfSamples, overlapping, config.signalWindow);

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

    DataMaxHolder dataMaxHolder(config.numberOfSamples, config.numberOfSignalsForMaxHold);
    DataAverager dataAverager(config.numberOfSamples, config.numberOfSignalsForAveraging);
    DataSmoother dataSmoother(config.numberOfSamples, config.alphaFactor);


    while(shouldProceed)
    {
        const auto &fftResult = fftDataExchanger.get();

        if(fftResult == nullptr)
        {
            continue;
        }

        statsManager.update();

        auto power = calculatePower(*fftResult);

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

    const WindowConfig windowConfig{config.horizontalSize,
                              config.verticalSize,
                              config.numberOfRectangles,
                              config.gapWidthInRelationToRectangleWidth,
                              config.smallRectangleHeightInPercentOfScreenSize,
                              config.colorsOfRectangle,
                              config.colorsOfSmallRectangle,
                              config.advancedColorSettings};

    std::unique_ptr<Window> window = std::make_unique<Window>(windowConfig);
    window->initializeGPU();

    IndexSelector indexSelector(config.samplingRate, config.numberOfSamples, config.frequencies);
    SmallRectanglesPositionCalculator smallRectanglesPositionCalculator(config.numberOfRectangles, config.speedOfFalling, config.accelerationStateOfFalling);

    while(shouldProceed)
    {
        const auto &data = processedDataExchanger.get();

        if(data == nullptr)
        {
            continue;
        }

        statsManager.update();

        std::vector<float> dataToBePrinted(config.numberOfRectangles,0);

        for(uint i=0;i<config.numberOfRectangles;++i)
        {
            dataToBePrinted[i]= config.scalingFactor*(data->at(indexSelector.getFrequencyIndex(i))+config.offsetFactor);
        }

        const auto smallRectanglesPosition = config.smallRectanglesVisibilityState ? smallRectanglesPositionCalculator.getPositions(dataToBePrinted) : std::vector<float>{};

        window->draw(dataToBePrinted, smallRectanglesPosition);

        if(window->checkIfWindowShouldBeClosed())
        {
            shouldProceed.store(false);
        }

        if(window->checkIfWindowShouldBeRecreated())
        {
            window = std::make_unique<Window>(windowConfig);
            window->initializeGPU();
        }

    }

}

void AudioSpectrumAnalyzer::flowController()
{
    float coeffUsedInCaseWhenScreenFallsBehindIncomingData = -0.01;

    auto previousTime = high_resolution_clock::now();

    while(shouldProceed)
    {
        std::this_thread::sleep_for(100ms);

        auto numberOfFramesPerSecond = StatsManager::getStatsFor("drafter").getNumberOfCallsInLast(1000ms);
        auto overlappingDiff = calculateOverlappingDiff(config.desiredFrameRate, numberOfFramesPerSecond);
        auto overlapping = calculateOverlapping(config.samplingRate, config.numberOfSamples, numberOfFramesPerSecond);

        if(processedDataExchanger.getSize() > 1)
        {
            overlapping = overlapping + coeffUsedInCaseWhenScreenFallsBehindIncomingData;
        }

        overlapping = overlapping + overlappingDiff;

        if((overlapping >=0) && (overlapping < 1))
        {
            flowControlDataExchanger.push_back(std::move(overlapping));
        }

        auto now = high_resolution_clock::now();

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
