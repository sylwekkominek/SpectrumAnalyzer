/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "helpers/WindowTestsBase.hpp"
#include "core/Window.hpp"


struct WindowTests  : public WindowTestsBase, public ::testing::Test
{
    static constexpr bool defaultFullscreenState = false;

    Configuration getConfig()
    {
        Configuration config{};

        uint32_t numberOfSamples = 2048;
        Frequencies frequencies = {100,200,300,400,500,600,700,800,900,1000};

        config.data.add(Freqs{frequencies});
        config.data.add(NumberOfRectangles{(uint16_t)frequencies.size()});
        config.data.add(NumberOfSamples{numberOfSamples});
        config.data.add(DesiredFrameRate{1});
        config.data.add(ColorOfStaticLines{{}});
        config.data.add(HorizontalLinePositions{{}});
        config.data.add(SamplingRate{44100});
        config.data.add(DynamicMaxHoldVisibilityState{true});
        config.data.add(DefaultFullscreenState{false});
        config.data.add(ScalingFactor{1});
        config.data.add(ColorsOfRectangle{});
        config.data.add(ColorsOfDynamicMaxHoldRectangle{});
        config.data.add(DynamicMaxHoldSpeedOfFalling{});
        config.data.add(DynamicMaxHoldAccelerationStateOfFalling{false});
        config.data.add(AdvancedColorSettings{});
        config.data.add(BackgroundColorSettings{});
        config.data.add(GapWidthInRelationToRectangleWidth{});
        config.data.add(DynamicMaxHoldRectangleHeightInPercentOfScreenSize{});


        return config;
    }
};

TEST_F(WindowTests, checkIfOpenGlFunctionsAreBeingCalled)
{
    const auto config = getConfig();
    const std::vector<float> signalToBeDrawn = std::vector<float>(config.data.get<NumberOfSamples>().value, 100);

    expectCreateWindow();
    expectInitializeGPU(config.data.get<NumberOfRectangles>().value, true);
    expectDraw(config.data.get<NumberOfRectangles>().value, true);
    expectCheckIfWindowShouldBeClosed();
    expectCheckIfWindowShouldRecreated();
    expectDestroyWindow();

    Window window(getConfig(),defaultFullscreenState);
    window.initializeGPU();
    window.draw(std::vector<float>(config.data.get<NumberOfSamples>().value, 0));

    window.checkIfWindowShouldBeClosed();
    window.checkIfWindowShouldBeRecreated();
}

