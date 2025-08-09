/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "core/gpu/TextInsideGpu.hpp"
#include <gmock/gmock.h>


class TextInsideGpuMock : public TextInsideGpu
{
public:
    using TextInsideGpu::TextInsideGpu;
    TextInsideGpuMock();

    MOCK_METHOD1(draw, void(const float));
    MOCK_METHOD0(initialize, void());
    MOCK_METHOD2(updateWindowSize, void(const float, const float));
    MOCK_METHOD0(finalize, void());
};
