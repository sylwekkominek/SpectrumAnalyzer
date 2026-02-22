/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct ColorOfDynamicMaxHoldLine
{
    ColorOfDynamicMaxHoldLine(const Color &value);
    ColorOfDynamicMaxHoldLine(const ThemeConfig themeConfig);
    std::string getInfo();
    Color value;
    const std::string name{"ColorOfDynamicMaxHoldLine"};
private:
    Color getColorOfDynamicMaxHoldLine(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const ColorOfDynamicMaxHoldLine &colorOfDynamicMaxHoldLine);
