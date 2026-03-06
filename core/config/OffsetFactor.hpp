/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct OffsetFactor
{
    OffsetFactor(float value);
    OffsetFactor(const ThemeConfig themeConfig, const Mode mode);
    std::string getInfo();
    float value;
    const std::string name{"OffsetFactor"};
private:
    template <Mode>
    float getOffsetFactor(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const OffsetFactor &offsetFactor);
