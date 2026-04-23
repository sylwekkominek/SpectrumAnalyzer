/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "StereoRmsMeter.hpp"
#include "dataSource/DataSourceBase.hpp"
#include "Stats.hpp"
#include "Helpers.hpp"
#include "CommonData.hpp"
#include "DataCalculator.hpp"
#include "FrequenciesInfo.hpp"
#include "FftBinCombiner.hpp"
#include <optional>


StereoRmsMeter::StereoRmsMeter(const Configuration &configuration, std::promise<AppEvent> &&appEvent):
    AudioSpectrumAnalyzerBase(configuration, std::move(appEvent))
{
}

void StereoRmsMeter::init()
{
    threads.push_back(std::thread(&StereoRmsMeter::samplesUpdater,this));
    threads.push_back(std::thread(&StereoRmsMeter::fftCalculator,this));
    threads.push_back(std::thread(&StereoRmsMeter::processing,this));
    threads.push_back(std::thread(&StereoRmsMeter::drafter,this));
    threads.push_back(std::thread(&StereoRmsMeter::flowController,this));
}

void StereoRmsMeter::fftCalculator()
{
    const std::string processName{"fftCalculator"};
    StatsManager statsManager(processName);

    float overlapping = calculateOverlapping(config.get<SamplingRate>(), config.get<NumberOfSamples>(), config.get<DesiredFrameRate>());

    WelchCalculator fftLeft(FftType::Real, config.get<NumberOfSamples>(), overlapping,  config.get<SignalWindow>());
    WelchCalculator fftRight(FftType::Real, config.get<NumberOfSamples>(), overlapping,  config.get<SignalWindow>());

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

        fftLeft.updateOverlapping(overlapping);
        fftRight.updateOverlapping(overlapping);
        fftLeft.updateBuffer(std::any_cast<StereoData>(*dataInTimeDomain).left);
        fftRight.updateBuffer(std::any_cast<StereoData>(*dataInTimeDomain).right);


        auto fftResultLeft = fftLeft.calculate();
        auto fftResultRight = fftRight.calculate();

        for(uint32_t i=0; i<std::min(fftResultLeft.size(), fftResultRight.size()); ++i)
        {
            fftDataExchanger.push_back(std::make_unique<std::any>(StereoFftData{std::move(fftResultLeft.at(i)), std::move(fftResultRight.at(i))}));
        }

    }

    fftDataExchanger.stop();
}

void StereoRmsMeter::processing()
{
    const std::string processName{"processing"};
    StatsManager statsManager(processName);

    FrequenciesInfo frequenciesInfo(config.get<SamplingRate>(), config.get<NumberOfSamples>(), config.get<Freqs>());

    DataMaxHolder dataMaxHolderLeft(1, config.get<NumberOfSignalsForMaxHold>(), getFloorDbFs16bit());
    DataAverager dataAveragerLeft(1, config.get<NumberOfSignalsForAveraging>());
    DataSmoother dataSmootherLeft(1, config.get<AlphaFactor>());
    FftBinCombiner fftBinCombinerLeft(config.get<ScalingFactor>(), config.get<OffsetFactor>(), frequenciesInfo.getAllFrequencyIndexes());

    DataMaxHolder dataMaxHolderRight(1, config.get<NumberOfSignalsForMaxHold>(), getFloorDbFs16bit());
    DataAverager dataAveragerRight(1, config.get<NumberOfSignalsForAveraging>());
    DataSmoother dataSmootherRight(1, config.get<AlphaFactor>());
    FftBinCombiner fftBinCombinerRight(config.get<ScalingFactor>(), config.get<OffsetFactor>(), frequenciesInfo.getAllFrequencyIndexes());


    while(shouldProceed)
    {
        const auto &fftResult = fftDataExchanger.get();

        if(fftResult == nullptr)
        {
            continue;
        }

        statsManager.update();


        dataMaxHolderLeft.push_back({fftBinCombinerLeft.combineRmsValues(std::any_cast<StereoFftData>(*fftResult).left)});
        dataMaxHolderRight.push_back({fftBinCombinerRight.combineRmsValues(std::any_cast<StereoFftData>(*fftResult).right)});

        auto dataWithMaxValueLeft = dataMaxHolderLeft.calculate();
        auto dataWithMaxValueRight = dataMaxHolderRight.calculate();

        if(!dataWithMaxValueLeft.empty() && !dataWithMaxValueRight.empty())
        {
            dataAveragerLeft.push_back(dataWithMaxValueLeft);
            dataAveragerRight.push_back(dataWithMaxValueRight);

            auto averagedDataLeft = dataAveragerLeft.calculate();
            auto averagedDataRight = dataAveragerRight.calculate();

            if(!averagedDataLeft.empty() && !averagedDataRight.empty())
            {
                dataSmootherLeft.push_back(averagedDataLeft);
                dataSmootherRight.push_back(averagedDataRight);
                auto smoothedDataLeft = dataSmootherLeft.calculate();
                auto smoothedDataRight = dataSmootherRight.calculate();


                processedDataExchanger.push_back(std::make_unique<std::any>(Data{getAverage(smoothedDataLeft), getAverage(smoothedDataRight)}));
            }
        }
    }

    processedDataExchanger.stop();
}

StereoRmsMeter::~StereoRmsMeter()
{
}
