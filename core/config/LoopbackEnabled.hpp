/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct LoopbackEnabled
{
    LoopbackEnabled(bool value);
    LoopbackEnabled(const ThemeConfig themeConfig, const Mode mode);
    std::string getInfo();
    bool value;
    const std::string name{"LoopbackEnabled"};
private:
    template <Mode>
    bool getLoopbackEnabled(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const LoopbackEnabled &loopbackEnabled);

