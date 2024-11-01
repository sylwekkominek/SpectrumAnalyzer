/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/AudioSpectrumAnalyzer.hpp"
#include "core/Stats.hpp"
#include "helpers/TestHelpers.hpp"
#include "helpers/WindowTestsBase.hpp"
#include "helpers/ValuesChecker.hpp"
#include <memory>
#include <gtest/gtest.h>


class AudioSpectrumAnalyzerTests : public ValuesChecker<-2>, public ::testing::Test
{
public:
    using Signal = std::vector<float>;
    using Signals = std::vector<Signal>;

    class ModifiedAudioSpectrumAnalyzer : public AudioSpectrumAnalyzer
    {
    public:
        using AudioSpectrumAnalyzer::AudioSpectrumAnalyzer;

        void init() override
        {
            StatsManager::clear();
            threads.push_back(std::thread(&ModifiedAudioSpectrumAnalyzer::samplesUpdater,this));
            threads.push_back(std::thread(&AudioSpectrumAnalyzer::fftCalculator,this));
            threads.push_back(std::thread(&AudioSpectrumAnalyzer::processing,this));
            threads.push_back(std::thread(&ModifiedAudioSpectrumAnalyzer::drafter,this));
            threads.push_back(std::thread(&ModifiedAudioSpectrumAnalyzer::statsPrinter,this));
        }

        void samplesUpdater() override
        {
            StatsManager statsManager("samplesUpdater");

            for(auto signalNo=0; signalNo < numberOfSignalsToBeTransferred; ++signalNo)
            {
                statsManager.update();
                const auto data1 = generateSignal(config.numberOfSamples,config.samplingRate,1000, dbFsToAmplitude(-signalNo));
                const auto data2 = generateSignal(config.numberOfSamples,config.samplingRate,2000, dbFsToAmplitude(-signalNo));
                const auto signal = addSignals(data1,data2);
                dataExchanger.push_back(std::make_unique<Data>(std::move(signal)));
            }

            while(shouldProceed)
            {
                std::this_thread::sleep_for(50ms);
            }

            dataExchanger.stop();
        }

        void drafter() override
        {
            StatsManager statsManager("drafter");

            auto dbFs=0;

            while(shouldProceed)
            {
                const auto &data = processedDataExchanger.get();

                if(data == nullptr)
                {
                    continue;
                }

                statsManager.update();
                valueChecker(*data, prepareExpectedFreqDomainSignal(dbFs--));
            }
            EXPECT_EQ(dbFs, -numberOfSignalsToBeTransferred);
        }

        void statsPrinter() override
        {
            while(shouldProceed)
            {
               std::this_thread::sleep_for(100ms);

               if(StatsManager::getStatsFor("drafter").getNumberOfCallsInLast(3000ms) < numberOfSignalsToBeTransferred)
               {
                  continue;
               }
               shouldProceed.store(false);
            }
        }
    };

    Configuration getConfig()
    {
        Configuration config{};
        config.numberOfSamples = 2048;
        config.samplingRate = 8000;
        config.overlapping = 0.0;
        config.numberOfSignalsForAveraging = 1;
        config.numberOfSignalsForMaxHold = 1;
        config.maxQueueSize = 100;
        return config;
    }

private:

    static Signal prepareExpectedFreqDomainSignal(const float fullScaleOffset)
    {
        const std::vector<uint> expectedFftPositions1000Hz = {253, 1789};
        const std::vector<uint> expectedFftPositions2000Hz = {509, 1533};
        std::vector<uint> expectedFftValuesPositions;

        expectedFftValuesPositions.insert(expectedFftValuesPositions.begin(),expectedFftPositions1000Hz.begin(),expectedFftPositions1000Hz.end());
        expectedFftValuesPositions.insert(expectedFftValuesPositions.begin(),expectedFftPositions2000Hz.begin(),expectedFftPositions2000Hz.end());

        std::vector<float> fftValuesWithFullScale{43.07, 66.56,78.11, 81.67,78.11,66.56,43.07};

        std::transform(fftValuesWithFullScale.begin(),fftValuesWithFullScale.end(),fftValuesWithFullScale.begin(),[&](const auto &el){
            return el+fullScaleOffset;
        });

        std::vector<float> fftSignal(2048);

        for(const auto &position: expectedFftValuesPositions)
        {
            std::copy(fftValuesWithFullScale.begin(), fftValuesWithFullScale.end(), fftSignal.begin() + position);
        }

        return fftSignal;
    }

    static constexpr float numberOfSignalsToBeTransferred{40};
};

TEST_F(AudioSpectrumAnalyzerTests, checkCalculationsAndDataTransfer)
{
    std::unique_ptr<SpectrumAnalyzerBase> spectrumAnalyzer = std::make_unique<ModifiedAudioSpectrumAnalyzer>(getConfig());
    spectrumAnalyzer->init();
    spectrumAnalyzer->run();
}


class AudioSpectrumAnalyzerTests2 : public WindowTestsBase, public ::testing::Test
{
public:
    using Signal = std::vector<float>;
    using Signals = std::vector<Signal>;

    class ModifiedAudioSpectrumAnalyzer : public AudioSpectrumAnalyzer
    {
    public:

        ModifiedAudioSpectrumAnalyzer(const Configuration &configuration): AudioSpectrumAnalyzer(configuration)
        {
            audioConfigFile="testAudioConfig";
        }

        void init() override
        {
            StatsManager::clear();
            threads.push_back(std::thread(&AudioSpectrumAnalyzer::samplesUpdater,this));
            threads.push_back(std::thread(&AudioSpectrumAnalyzer::fftCalculator,this));
            threads.push_back(std::thread(&AudioSpectrumAnalyzer::processing,this));
            threads.push_back(std::thread(&AudioSpectrumAnalyzer::drafter,this));
            threads.push_back(std::thread(&ModifiedAudioSpectrumAnalyzer::statsPrinter,this));
        }

        void statsPrinter() override
        {
            while(shouldProceed)
            {
               std::this_thread::sleep_for(50ms);

               if(StatsManager::getStatsFor("drafter").getNumberOfCallsInLast(3000ms) < numberOfSignalsToBeTransferred)
               {
                  continue;
               }
               shouldProceed.store(false);
            }
        }
    };

    Configuration getConfig()
    {
        Configuration config{};
        config.frequencies = {988,992,996,1000,1004,1008,1012};
        config.numberOfRectangles = config.frequencies.size();
        config.numberOfSamples = 2048;
        config.samplingRate = 8000;
        config.overlapping = 0.0;
        config.numberOfSignalsForAveraging = 1;
        config.numberOfSignalsForMaxHold = 1;
        config.maxQueueSize = 5;
        config.smallRectanglesVisibilityState = true;
        config.scalingFactor = 1.0;
        config.colorsOfRectangle = {{0,{1,1,1}}, {1,{1,1,1}}, {2,{1,1,1}}, {3,{1,1,1}}};
        config.colorsOfSmallRectangle = config.colorsOfRectangle;
        return config;
    }

    void expectWindowDraw(const Configuration &config)
    {
        expectCreateWindow();
        expectInitializeGPU(config.numberOfRectangles, true);
        expectDraw({-1.19859, -0.72883, -0.497998, -0.426634, -0.497998, -0.72883, -1.19859}, true);
        expectCheckIfWindowShouldBeClosed();
        expectDestroyWindow();
    }

private:
    static constexpr float numberOfSignalsToBeTransferred{1};
};


TEST_F(AudioSpectrumAnalyzerTests2, checkCalculationsAndDataTransfer)
{
    auto config = getConfig();

    expectWindowDraw(config);
    std::unique_ptr<SpectrumAnalyzerBase> spectrumAnalyzer = std::make_unique<ModifiedAudioSpectrumAnalyzer>(getConfig());
    spectrumAnalyzer->init();
    spectrumAnalyzer->run();
}
