/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "helpers/WindowTestsBase.hpp"
#include "core/Window.hpp"


struct WindowTests  : public WindowTestsBase, public ::testing::Test
{
};

TEST_F(WindowTests, checkIfOpenGlFunctionsAreBeingCalled)
{
    auto numberOfRectangles{10};
    auto gap{0.1};

    expectCreateWindow();
    expectInitializeGPU(numberOfRectangles,true);
    expectDraw(numberOfRectangles,true);
    expectCheckIfWindowShouldBeClosed();
    expectDestroyWindow();

    WindowConfig windowConfig{1920,1080, 10,gap,1, {{0, {1,1,1}},{1, {1,1,1}},{2,{1,1,1}}, {3,{1,1,1}}},{{0, {1,1,1}},{1, {1,1,1}},{2,{1,1,1}}, {3,{1,1,1}}}, {}};
    Window window(windowConfig);
    window.initializeGPU();
    window.draw({1,2,3,4,5,6,7,8,9,10}, {20,21,23,24,25,26,27,28,29,30});

    window.checkIfWindowShouldBeClosed();
}

