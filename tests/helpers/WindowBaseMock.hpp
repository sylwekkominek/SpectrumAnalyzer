/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "core/WindowBase.hpp"
#include <gmock/gmock.h>

class WindowBaseMock : public WindowBase
{
public:
    WindowBaseMock();

    MOCK_METHOD0(createWindow, void());
    MOCK_METHOD0(checkIfWindowShouldBeClosed, bool());
    MOCK_METHOD0(checkIfWindowShouldBeRecreated, bool());
    MOCK_METHOD0(getUpdatedThemeNumber, std::optional<uint16_t>());
    MOCK_METHOD0(swapBuffers, void());
    MOCK_METHOD0(getCursorPosition, CursorPosition());
    MOCK_METHOD0(getWindowSize, WindowSize());
};
