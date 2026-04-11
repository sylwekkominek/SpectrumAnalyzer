/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "ConfigReader.hpp"
#include "DataExchanger.hpp"
#include "FftCalculator.hpp"
#include <vector>
#include <thread>
#include <atomic>
#include <future>
#include <variant>
#include <any>

using AppEvent = std::variant<ThemeConfig, ApplicationState>;

class SpectrumAnalyzerBase
{
public:

    SpectrumAnalyzerBase(const Configuration &configuration, std::promise<AppEvent> &&appEvent):
        config(configuration),
        shouldProceed(true),
        appEventFuture(appEvent.get_future()),
        appEventPromise(std::promise<AppEvent>(std::move(appEvent))),
        dataExchanger(configuration.get<MaxQueueSize>()),
        fftDataExchanger(configuration.get<MaxQueueSize>()),
        processedDataExchanger(configuration.get<MaxQueueSize>()),
        flowControlDataExchanger(maxQueueSizeForFlowController)
    {
    }

    void run()
    {
        for(auto & thread : threads)
        {
            if(thread.joinable())
            {
                thread.join();
            }
        }
    }

    AppEvent getEvent()
    {
        return appEventFuture.get();
    }

    virtual void init() =0;
    virtual void samplesUpdater() =0;
    virtual void fftCalculator() =0;
    virtual void processing() =0;
    virtual void drafter() =0;
    virtual void flowController() =0;


    virtual ~SpectrumAnalyzerBase()
    {
    }

protected:


    using Data = std::vector<float>;
    const Configuration config;

    std::atomic<bool> shouldProceed;
    std::future<AppEvent> appEventFuture;
    std::promise<AppEvent> appEventPromise;



    DataExchanger<std::unique_ptr<std::any>> dataExchanger;
    DataExchanger<std::unique_ptr<std::any>> fftDataExchanger;
    DataExchanger<std::unique_ptr<std::any>> processedDataExchanger;
    DataExchanger<float> flowControlDataExchanger;
    std::vector<std::thread> threads;
private:
    static constexpr uint32_t maxQueueSizeForFlowController = 1;
};
