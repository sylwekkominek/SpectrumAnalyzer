/*
 * Copyright (C) 2024-2025, Sylwester Kominek
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
            threads.push_back(std::thread(&ModifiedAudioSpectrumAnalyzer::flowController,this));
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

        void flowController() override
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
        config.desiredFrameRate = 1;
        config.numberOfSignalsForAveraging = 1;
        config.numberOfSignalsForMaxHold = 1;
        config.alphaFactor = 1;
        config.maxQueueSize = 100;
        config.signalWindow = getSignalWindow(config.numberOfSamples);
        return config;
    }

private:

    static Signal prepareExpectedFreqDomainSignal(const float fullScaleOffset)
    {
        const std::vector<uint> expectedFftPositions1000Hz = {254, 1790};
        const std::vector<uint> expectedFftPositions2000Hz = {510, 1534};
        std::vector<uint> expectedFftValuesPositions;

        expectedFftValuesPositions.insert(expectedFftValuesPositions.begin(),expectedFftPositions1000Hz.begin(),expectedFftPositions1000Hz.end());
        expectedFftValuesPositions.insert(expectedFftValuesPositions.begin(),expectedFftPositions2000Hz.begin(),expectedFftPositions2000Hz.end());

        std::vector<float> fftValuesWithFullScale{62.37,78.27,82.77,78.27,62.37};

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
            threads.push_back(std::thread(&ModifiedAudioSpectrumAnalyzer::flowController,this));
        }

        void flowController() override
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
        config.frequencies = {992,996,1000,1004,1008};
        config.numberOfRectangles = config.frequencies.size();
        config.numberOfSamples = 2048;
        config.signalWindow = getSignalWindow(config.numberOfSamples);
        config.samplingRate = 8000;
        config.desiredFrameRate = 1;
        config.numberOfSignalsForAveraging = 1;
        config.numberOfSignalsForMaxHold = 1;
        config.alphaFactor = 1;
        config.maxQueueSize = 5;
        config.dynamicMaxHoldVisibilityState = true;
        config.scalingFactor = 1.0;
        config.gapWidthInRelationToRectangleWidth = 0;
        config.colorsOfRectangle = {{0,{1,1,1,1}}, {1,{1,1,1,1}}, {2,{1,1,1,1}}, {3,{1,1,1,1}}};
        config.colorsOfDynamicMaxHoldRectangle = config.colorsOfRectangle;
        return config;
    }

    void expectWindowDraw(const Configuration &config)
    {
        expectCreateWindow();
        expectInitializeGPU(config.numberOfRectangles, true);
        expectDraw({-0.812812, -0.494849, -0.404816, -0.494849,-0.812812}, true);
        expectCheckIfWindowShouldBeClosed();
        expectCheckIfWindowShouldRecreated();
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

