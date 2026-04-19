/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/AudioSpectrumAnalyzer.hpp"
#include "core/Stats.hpp"
#include "dataSource/DataSourceBase.hpp"
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
                const auto leftData = generateSignal(config.data.get<NumberOfSamples>().value,config.data.get<SamplingRate>().value,1000, dbFsToAmplitude(-signalNo));
                const auto rightData = generateSignal(config.data.get<NumberOfSamples>().value,config.data.get<SamplingRate>().value,2000, dbFsToAmplitude(-signalNo));
                dataExchanger.push_back(std::make_unique<std::any>(StereoData{std::move(leftData), std::move(rightData)}));
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

                valueChecker(std::any_cast<Data>(*data), prepareExpectedFreqDomainSignal(dbFs--));
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


        uint32_t numberOfSamples = 2048;
        uint32_t sampleRate = 8000;

        config.data.add(NumberOfSamples{numberOfSamples});
        config.data.add(SamplingRate{sampleRate});
        config.data.add(DesiredFrameRate{1});
        config.data.add(NumberOfSignalsForAveraging{1});
        config.data.add(NumberOfSignalsForMaxHold{1});
        config.data.add(AlphaFactor{1});
        config.data.add(MaxQueueSize{100});
        config.data.add(SignalWindow{getSignalWindow(numberOfSamples)});
        config.data.add(ScalingFactor{1});
        config.data.add(OffsetFactor{0});
        config.data.add(Freqs(getDemandedFrequencies(sampleRate, numberOfSamples, 0, numberOfSamples/2)));
        return config;
    }

private:

    static Signal prepareExpectedFreqDomainSignal(const float fullScaleOffset, const float defaultValue = -96.32)
    {

        const std::vector<uint32_t> expectedFftPositions1000Hz = {254, 1790};
        const std::vector<uint32_t> expectedFftPositions2000Hz = {510, 1534};
        std::vector<uint32_t> expectedFftValuesPositions;

        expectedFftValuesPositions.insert(expectedFftValuesPositions.end(),expectedFftPositions1000Hz.begin(),expectedFftPositions1000Hz.end());
        expectedFftValuesPositions.insert(expectedFftValuesPositions.end(),expectedFftPositions2000Hz.begin(),expectedFftPositions2000Hz.end());

        std::vector<float> fftValuesWithFullScale{-33.9599,-18.0616, -13.5599 ,-18.0614, -33.9593};

        std::transform(fftValuesWithFullScale.begin(),fftValuesWithFullScale.end(),fftValuesWithFullScale.begin(),[&](const auto &el){
            return el+fullScaleOffset;
        });

        std::vector<float> fftSignal(1024, defaultValue);

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

        uint32_t numberOfSamples = 2048;
        Frequencies frequencies = {992,996,1000,1004,1008};
        const ThemeConfig theme = ThemeConfig::Theme1;
        const Mode mode = Mode::StereoRmsMeter;

        config.data.add(Freqs{frequencies});
        config.data.add(VerticalLinePositions{Frequencies{}});
        config.data.add(FrequencyTextPositions{Frequencies{}});
        config.data.add(NumberOfRectangles{(uint16_t)frequencies.size()});
        config.data.add(PythonDataSourceEnabled{true});
        config.data.add(NumberOfSamples{numberOfSamples});
        config.data.add(SamplingRate{8000});
        config.data.add(DesiredFrameRate{1});
        config.data.add(NumberOfSignalsForAveraging{1});
        config.data.add(NumberOfSignalsForMaxHold{1});
        config.data.add(AlphaFactor{1});
        config.data.add(MaxQueueSize{5});
        config.data.add(SignalWindow{getSignalWindow(numberOfSamples)});
        config.data.add(ScalingFactor{1});
        config.data.add(DynamicMaxHoldVisibilityState{true});
        config.data.add(GapWidthInRelationToRectangleWidth{0});
        config.data.add(ColorsOfRectangle{theme, mode});
        config.data.add(ColorsOfDynamicMaxHoldRectangle{theme, mode});
        config.data.add(DefaultFullscreenState{false});
        config.data.add(HorizontalLinePositions{Positions{}});
        config.data.add(DynamicMaxHoldSpeedOfFalling{900});
        config.data.add(DynamicMaxHoldSecondarySpeedOfFalling{900});
        config.data.add(DynamicMaxHoldAccelerationStateOfFalling{false});
        config.data.add(DynamicMaxHoldAccelerationStateOfFalling{false});
        config.data.add(AdvancedColorSettings{theme, mode});
        config.data.add(BackgroundColorSettings{theme, mode});
        config.data.add(GapWidthInRelationToRectangleWidth{theme, mode});
        config.data.add(DynamicMaxHoldRectangleHeightInPercentOfScreenSize{theme, mode});
        config.data.add(ColorOfStaticText{Color{1,1,1,1}});
        config.data.add(ColorOfLine{Color{1,1,1,1}});
        config.data.add(ColorOfStaticLines{Color{1,1,1,1}});
        config.data.add(ColorOfDynamicMaxHoldLine{Color{1,1,1,1}});
        config.data.add(ColorOfDynamicMaxHoldSecondaryLine{Color{1,1,1,1}});
        config.data.add(ColorsOfDynamicMaxHoldSecondaryRectangle{theme, mode});
        config.data.add(OffsetFactor{theme, mode});
        config.data.add(RectanglesVisibilityState{true});
        config.data.add(LinesVisibilityState{false});
        config.data.add(DynamicMaxHoldSecondaryVisibilityState{true});
        config.data.add(LoopbackEnabled{false});
        config.data.add(HorizontalDrawingArea{{5,90}});
        config.data.add(VerticalDbfsRange{{-96.32, 0}});
        config.data.add(SingleScaleMode{false});

        return config;
    }

    void expectWindowDraw(const Configuration &config)
    {
        expectCreateWindow();
        expectInitializeGPU(config.get<NumberOfRectangles>(), true);
        expectDraw({-0.632012, -0.301901,  -0.208429,-0.301901, -0.632012}, true);
        expectCheckIfWindowShouldBeClosed();
        expectCheckIfWindowShouldRecreated();
        expectCheckIfThemeShouldBeChanged();
        expectDestroyWindow();
    }

private:
    static constexpr float numberOfSignalsToBeTransferred{1};
};


TEST_F(AudioSpectrumAnalyzerTests2, checkCalculationsAndDataTransfer)
{
    auto config = getConfig();

    expectWindowDraw(config);
    std::unique_ptr<SpectrumAnalyzerBase> spectrumAnalyzer = std::make_unique<ModifiedAudioSpectrumAnalyzer>(config);
    spectrumAnalyzer->init();
    spectrumAnalyzer->run();

    EXPECT_EQ(std::get<ApplicationState>(spectrumAnalyzer->getEvent()), ApplicationState::Shutdown);
}

