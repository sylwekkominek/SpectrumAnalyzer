/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <cstdint>
#include <ostream>

struct SamplingRate
{
    SamplingRate(uint32_t value);
    SamplingRate(const ThemeConfig themeConfig);
    std::string getInfo();
    uint32_t value;
    const std::string name{"SamplingRate"};
private:
    uint32_t getSamplingRate(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const SamplingRate &samplingRate);

