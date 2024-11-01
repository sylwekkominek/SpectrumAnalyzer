/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "ConfigReader.hpp"
#include "DataExchanger.hpp"
#include "FFTCalculator.hpp"
#include <vector>
#include <thread>
#include <atomic>


class SpectrumAnalyzerBase
{
public:

    SpectrumAnalyzerBase(const Configuration &configuration):
        config(configuration),
        shouldProceed(true),
        dataExchanger(configuration.maxQueueSize),
        fftDataExchanger(configuration.maxQueueSize),
        processedDataExchanger(configuration.maxQueueSize)
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

    virtual void init() =0;
    virtual void samplesUpdater() =0;
    virtual void fftCalculator() =0;
    virtual void processing() =0;
    virtual void drafter() =0;
    virtual void statsPrinter() =0;

    virtual ~SpectrumAnalyzerBase()
    {
    }

protected:

    using Data = std::vector<float>;
    const Configuration config;

    std::atomic<bool> shouldProceed;
    DataExchanger<std::unique_ptr<Data>> dataExchanger;
    DataExchanger<std::unique_ptr<FFTResult>> fftDataExchanger;
    DataExchanger<std::unique_ptr<Data>> processedDataExchanger;
    std::vector<std::thread> threads;
};
