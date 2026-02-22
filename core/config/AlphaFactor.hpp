/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct AlphaFactor
{
    AlphaFactor(float value);
    AlphaFactor(const ThemeConfig themeConfig);

    std::string getInfo();
    float value;
    const std::string name{"AlphaFactor"};

private:
    float getAlphaFactor(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const AlphaFactor &alphaFactor);
