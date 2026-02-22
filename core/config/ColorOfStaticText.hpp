/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct ColorOfStaticText
{
    ColorOfStaticText(const Color &value);
    ColorOfStaticText(const ThemeConfig themeConfig);
    std::string getInfo();
    Color value;
    const std::string name{"ColorOfStaticText"};
private:
    Color getColorOfStaticText(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const ColorOfStaticText &colorOfStaticText);

