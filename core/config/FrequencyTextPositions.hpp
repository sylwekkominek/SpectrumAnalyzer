/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct FrequencyTextPositions
{
    FrequencyTextPositions(const Frequencies &value);
    FrequencyTextPositions(const ThemeConfig themeConfig, const Mode mode);
    std::string getInfo();
    Frequencies value;
    const std::string name{"FrequencyTextPositions"};
private:
    template <Mode>
    Frequencies getFrequencyTextPositions(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const FrequencyTextPositions &frequencyTextPositions);


