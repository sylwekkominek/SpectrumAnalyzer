/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct VerticalDbfsRange
{
    VerticalDbfsRange(std::pair<float, float> value);
    VerticalDbfsRange(const ThemeConfig themeConfig, const Mode mode);
    std::string getInfo();
    std::pair<float, float> value;
    const std::string name{"VerticalDbfsRange"};
private:
    template <Mode>
    std::pair<float, float> getVerticalDbfsRange(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const VerticalDbfsRange &verticalDbfsRange);

