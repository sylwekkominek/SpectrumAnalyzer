/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>
#include <ostream>


struct ColorOfLine
{
    ColorOfLine(const Color &value);
    ColorOfLine(const ThemeConfig themeConfig, const Mode mode);
    std::string getInfo();
    Color value;
    const std::string name{"ColorOfLine"};
private:
    template <Mode>
    Color getColorOfLine(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const ColorOfLine &colorOfLine);
