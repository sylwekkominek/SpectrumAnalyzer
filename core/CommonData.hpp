/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once


constexpr float getDynamicRangeOf16bitSignal()
{
    return 96.32;
}

constexpr float getFloorDbFs16bit()
{
    return -96.32;
}
