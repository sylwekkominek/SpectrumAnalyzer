/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioSpectrumAnalyzer.hpp"
#include "SamplesCollector.hpp"
#include "ConfigReader.hpp"
#include "Window.hpp"
#include "ChebWindow.hpp"
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
    threads.push_back(std::thread(&AudioSpectrumAnalyzer::statsPrinter,this));
}

void AudioSpectrumAnalyzer::samplesUpdater()
{
    const std::string processName{"samplesUpdater"};
    StatsManager statsManager(processName);

    SamplesCollector samplesCollector(audioConfigFile.c_str());
    samplesCollector.initialize(config.numberOfSamples,config.samplingRate);

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

    WelchCalculator fft(config.numberOfSamples, config.overlapping, getChebWindow(config.numberOfSamples));

    using namespace std::chrono;

    while(shouldProceed)
    {
        const auto &dataInTimeDomain  = dataExchanger.get();

        if(dataInTimeDomain == nullptr)
        {
            continue;
        }

        statsManager.update();

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

    DataAverager dataAverager(config.numberOfSamples, config.numberOfSignalsForAveraging);
    DataMaxHolder dataMaxHolder(config.numberOfSamples, config.numberOfSignalsForMaxHold);

    std::vector<float> lastAveragedData(config.numberOfSamples, 0);

    while(shouldProceed)
    {
        const auto &fftResult = fftDataExchanger.get();

        if(fftResult == nullptr)
        {
            continue;
        }

        statsManager.update();

        auto power = calculatePower(*fftResult);

        dataAverager.push_back(power);

        auto averagedData = dataAverager.calculateWithMoving();

        if(not averagedData.empty())
        {
            dataMaxHolder.push_back(averagedData);
            auto dataWithMaxValue = dataMaxHolder.calculateWithMoving();

            if(not dataWithMaxValue.empty())
            {
                lastAveragedData = dataWithMaxValue;
                processedDataExchanger.push_back(std::make_unique<Data>(std::move(dataWithMaxValue)));
            }
        }

    }

    processedDataExchanger.stop();
}

void AudioSpectrumAnalyzer::drafter()
{
    const std::string processName{"drafter"};
    StatsManager statsManager(processName);
    Window window(config.horizontalSize, config.verticalSize, config.numberOfRectangles, config.colorsOfRectangle, config.colorsOfSmallRectangle);
    window.initializeGPU();

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

        window.draw(dataToBePrinted, smallRectanglesPosition);

        if(window.checkIfWindowShouldBeClosed())
        {
            break;
        }
    }

}

void AudioSpectrumAnalyzer::statsPrinter()
{
    std::optional<time_point<high_resolution_clock>> timeWhenProgramWillBeClosed;
    auto lowRefreshRateTimeThresholdIsSeconds{3s};


    while(shouldProceed)
    {
       std::this_thread::sleep_for(1000ms);

       auto statsForSamplesUpdater = StatsManager::getStatsFor("samplesUpdater");
       auto statsForDrafter = StatsManager::getStatsFor("drafter");

       auto numberOfFramesPerSecond = statsForDrafter.getNumberOfCallsInLast(1000ms);


       if(numberOfFramesPerSecond > 1)
       {
           std::cout<<"Samples are updated: "<<statsForSamplesUpdater.getNumberOfCallsInLast(1000ms)<<" per second"<<std::endl;
           std::cout<<"Plots are updated: "<<numberOfFramesPerSecond<<" per second"<<std::endl;
           timeWhenProgramWillBeClosed.reset();
           continue;
       }


      if(!timeWhenProgramWillBeClosed)
      {
         timeWhenProgramWillBeClosed = high_resolution_clock::now()+lowRefreshRateTimeThresholdIsSeconds;
      }

      auto programWillBeClosedInSeconds = duration_cast<seconds>(*timeWhenProgramWillBeClosed - high_resolution_clock::now()) ;

      if(programWillBeClosedInSeconds  <= 0s)
      {
          shouldProceed.store(false);
      }

      std::cout<<"Program will be closed in: "<<  programWillBeClosedInSeconds.count()<<" seconds..."<<std::endl;

    }
}

AudioSpectrumAnalyzer::~AudioSpectrumAnalyzer()
{
}
