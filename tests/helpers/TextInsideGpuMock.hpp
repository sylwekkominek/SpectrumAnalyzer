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

    MOCK_METHOD2(draw, void(const HorizontalAligment, const VerticalAligment));
    MOCK_METHOD4(draw, void(const HorizontalAligment, const VerticalAligment verticalAligment, const float, const float));
    MOCK_METHOD5(draw, void(const std::string &, const HorizontalAligment, const VerticalAligment verticalAligment, const float, const float));
    MOCK_METHOD0(initialize, void());
    MOCK_METHOD0(finalize, void());
};


