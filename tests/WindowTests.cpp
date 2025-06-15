/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "helpers/WindowTestsBase.hpp"
#include "core/Window.hpp"


struct WindowTests  : public WindowTestsBase, public ::testing::Test
{
    Configuration getConfig()
    {
        Configuration config{};
        config.frequencies = {100,200,300,400,500,600,700,800,900,1000};
        config.numberOfRectangles = config.frequencies.size();
        config.numberOfSamples = 2048;
        config.samplingRate = 44100;
        config.dynamicMaxHoldVisibilityState = true;
        config.scalingFactor = 1.0;
        config.colorsOfRectangle ={{0, {1,1,1,1}},{1, {1,1,1,1}},{2,{1,1,1,1}}, {3,{1,1,1,1}}};
        config.colorsOfDynamicMaxHoldRectangle = {{0, {1,1,1,1}},{1, {1,1,1,1}},{2,{1,1,1,1}}, {3,{1,1,1,1}}};
        return config;
    }
};

TEST_F(WindowTests, checkIfOpenGlFunctionsAreBeingCalled)
{
    const auto config = getConfig();
    const std::vector<float> signalToBeDrawn = std::vector<float>(config.numberOfSamples, 100);

    expectCreateWindow();
    expectInitializeGPU(config.numberOfRectangles,true);
    expectDraw(config.numberOfRectangles,true);
    expectCheckIfWindowShouldBeClosed();
    expectCheckIfWindowShouldRecreated();
    expectDestroyWindow();

    Window window(getConfig());
    window.initializeGPU();
    window.draw(std::vector<float>(config.numberOfSamples,0));

    window.checkIfWindowShouldBeClosed();
    window.checkIfWindowShouldBeRecreated();
}

