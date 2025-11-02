/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "ConfigReader.hpp"
#include "CommonTypes.hpp"
#include <memory>


class WindowBase
{
public:
    WindowBase(const Configuration &config, const bool fullScreenEnabled);
    void createWindow();
    bool checkIfWindowShouldBeClosed();
    bool checkIfWindowShouldBeRecreated();
    ~WindowBase();

protected:

    void swapBuffers();

    CursorPosition getCursorPosition();
    WindowSize getWindowSize();

    const Configuration config{};
    const bool isFullScreenEnabled{};
    static constexpr float fullScreenSizeInPercents{100};

private:
    class WindowBaseImpl;
    std::unique_ptr<WindowBaseImpl> windowBaseImpl;
};

