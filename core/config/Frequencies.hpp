/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct Freqs
{
    Freqs(const Frequencies &value);
    Freqs(const ThemeConfig themeConfig, const Mode mode, uint16_t sampleRate, uint16_t fftSize);
    std::string getInfo();
    Frequencies value;
    const std::string name{"Frequencies"};
private:
    template <Mode>
    Frequencies getFreqs(const ThemeConfig themeConfig);
    uint16_t sampleRate;
    uint16_t fftSize;
};

std::ostream& operator<<(std::ostream& os, const Freqs &freqs);


