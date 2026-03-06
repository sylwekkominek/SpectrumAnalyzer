/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "helpers/WindowTestsBase.hpp"
#include "core/Window.hpp"


struct WindowTests  : public WindowTestsBase, public ::testing::Test
{
    static constexpr bool defaultFullscreenState = false;
    const ThemeConfig theme = ThemeConfig::Theme1;
    const Mode mode = Mode::Analyzer;

    Configuration getConfig()
    {
        Configuration config{};

        uint32_t numberOfSamples = 2048;
        Frequencies frequencies = {100,200,300,400,500,600,700,800,900,1000};

        config.data.add(Freqs{frequencies});
        config.data.add(VerticalLinePositions{Frequencies{}});
        config.data.add(FrequencyTextPositions{Frequencies{}});
        config.data.add(NumberOfRectangles{(uint16_t)frequencies.size()});
        config.data.add(NumberOfSamples{numberOfSamples});
        config.data.add(DesiredFrameRate{1});
        config.data.add(ColorOfStaticLines{Color{}});
        config.data.add(ColorOfStaticText{Color{}});
        config.data.add(HorizontalLinePositions{Positions{}});
        config.data.add(SamplingRate{44100});
        config.data.add(RectanglesVisibilityState{true});
        config.data.add(LinesVisibilityState{false});
        config.data.add(DynamicMaxHoldVisibilityState{true});
        config.data.add(DefaultFullscreenState{false});
        config.data.add(ScalingFactor{1});
        config.data.add(ColorsOfRectangle{theme, mode});
        config.data.add(ColorsOfDynamicMaxHoldRectangle{theme, mode});
        config.data.add(ColorsOfDynamicMaxHoldSecondaryRectangle{theme, mode});
        config.data.add(DynamicMaxHoldSpeedOfFalling{theme, mode});
        config.data.add(DynamicMaxHoldSecondarySpeedOfFalling{theme, mode});
        config.data.add(DynamicMaxHoldAccelerationStateOfFalling{false});
        config.data.add(AdvancedColorSettings{theme, mode});
        config.data.add(BackgroundColorSettings{theme, mode});
        config.data.add(GapWidthInRelationToRectangleWidth{theme, mode});
        config.data.add(DynamicMaxHoldRectangleHeightInPercentOfScreenSize{theme, mode});
        config.data.add(DynamicMaxHoldSecondaryVisibilityState{true});

        return config;
    }
};

TEST_F(WindowTests, checkIfOpenGlFunctionsAreBeingCalled)
{
    const auto config = getConfig();

    expectCreateWindow();
    expectInitializeGPU(config.data.get<NumberOfRectangles>().value, true);
    expectDraw(config.data.get<NumberOfRectangles>().value, true);
    expectCheckIfWindowShouldBeClosed();
    expectCheckIfWindowShouldRecreated();
    expectDestroyWindow();

    Window window(config,defaultFullscreenState);
    window.initializeGPU();
    window.draw(std::vector<float>(config.data.get<NumberOfSamples>().value, 0));

    window.checkIfWindowShouldBeClosed();
    window.checkIfWindowShouldBeRecreated();
}

