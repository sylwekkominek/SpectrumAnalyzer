/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/dataSource/DataSourceBase.hpp"
#include "core/StereoRmsMeter.hpp"
#include "core/Stats.hpp"
#include "helpers/TestHelpers.hpp"
#include "helpers/WindowTestsBase.hpp"
#include "helpers/ValuesChecker.hpp"
#include <memory>
#include <gtest/gtest.h>


class StereoRmsMeterTests : public ValuesChecker<-2>, public ::testing::Test
{
public:
    using Signal = std::vector<float>;
    using Signals = std::vector<Signal>;

    class ModifiedStereoRmsMeter : public StereoRmsMeter
    {
    public:
        using StereoRmsMeter::StereoRmsMeter;


        void init() override
        {
            StatsManager::clear();
            threads.push_back(std::thread(&ModifiedStereoRmsMeter::samplesUpdater,this));
            threads.push_back(std::thread(&StereoRmsMeter::fftCalculator,this));
            threads.push_back(std::thread(&StereoRmsMeter::processing,this));
            threads.push_back(std::thread(&ModifiedStereoRmsMeter::drafter,this));
            threads.push_back(std::thread(&ModifiedStereoRmsMeter::flowController,this));
        }

        void samplesUpdater() override
        {
            StatsManager statsManager("samplesUpdater");

            for(auto signalNo=0; signalNo < numberOfSignalsToBeTransferred; ++signalNo)
            {
                statsManager.update();
                const auto leftSignal = generateSignal(config.data.get<NumberOfSamples>().value,config.data.get<SamplingRate>().value, 1000, dbFsToAmplitude(-signalNo));
                const auto rightSignal = generateSignal(config.data.get<NumberOfSamples>().value,config.data.get<SamplingRate>().value, 2000, dbFsToAmplitude(-signalNo+offsetInDbBetweenLeftAndRight));

                dataExchanger.push_back(std::make_unique<std::any>(StereoData{leftSignal, rightSignal}));
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

                valueChecker(std::any_cast<Data>(*data), prepareExpectedRmsData(dbFs--));
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


        uint32_t numberOfSamples = 4096;
        uint32_t sampleRate = 44100;

        config.data.add(NumberOfRectangles{2});
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
        config.data.add(Freqs({20,20000}));
        return config;
    }

private:

    static Signal prepareExpectedRmsData(const float fullScaleOffset)
    {
        float leftExpectedValue = -8.17406 + fullScaleOffset;
        float rightExpectedValue = -8.17406 + offsetInDbBetweenLeftAndRight + fullScaleOffset;

        return {leftExpectedValue, rightExpectedValue};
    }

    static constexpr float numberOfSignalsToBeTransferred{40};
    static constexpr float offsetInDbBetweenLeftAndRight = -6;
};

TEST_F(StereoRmsMeterTests, checkCalculationsAndDataTransfer)
{
    std::unique_ptr<SpectrumAnalyzerBase> spectrumAnalyzer = std::make_unique<ModifiedStereoRmsMeter>(getConfig());
    spectrumAnalyzer->init();
    spectrumAnalyzer->run();
}

class StereoRmsMeterTests2 : public WindowTestsBase, public ::testing::Test
{
public:
    using Signal = std::vector<float>;
    using Signals = std::vector<Signal>;

    class ModifiedStereoRmsMeter : public StereoRmsMeter
    {
    public:

        ModifiedStereoRmsMeter(const Configuration &configuration): StereoRmsMeter(configuration)
        {
            audioConfigFile="testAudioConfig";
        }

        void init() override
        {
            StatsManager::clear();
            threads.push_back(std::thread(&StereoRmsMeter::samplesUpdater,this));
            threads.push_back(std::thread(&StereoRmsMeter::fftCalculator,this));
            threads.push_back(std::thread(&StereoRmsMeter::processing,this));
            threads.push_back(std::thread(&StereoRmsMeter::drafter,this));
            threads.push_back(std::thread(&ModifiedStereoRmsMeter::flowController,this));
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
        Frequencies frequencies = {20,20000};
        const ThemeConfig theme = ThemeConfig::Theme1;
        const Mode mode = Mode::Analyzer;

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
        expectDraw({-0.169749, -0.294756}, true);
        expectCheckIfWindowShouldBeClosed();
        expectCheckIfWindowShouldRecreated();
        expectCheckIfThemeShouldBeChanged();
        expectDestroyWindow();
    }

private:
    static constexpr float numberOfSignalsToBeTransferred{1};
};

TEST_F(StereoRmsMeterTests2, checkCalculationsAndDataTransfer)
{
    auto config = getConfig();

    expectWindowDraw(config);
    std::unique_ptr<SpectrumAnalyzerBase> spectrumAnalyzer = std::make_unique<ModifiedStereoRmsMeter>(getConfig());
    spectrumAnalyzer->init();
    spectrumAnalyzer->run();

    EXPECT_EQ(std::get<ApplicationState>(spectrumAnalyzer->getEvent()), ApplicationState::Shutdown);
}
