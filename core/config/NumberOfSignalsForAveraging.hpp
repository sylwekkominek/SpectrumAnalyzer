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

struct NumberOfSignalsForAveraging
{
    NumberOfSignalsForAveraging(uint32_t value);
    NumberOfSignalsForAveraging(const ThemeConfig themeConfig, const Mode mode);
    std::string getInfo();
    uint32_t value;
    const std::string name{"NumberOfSignalsForAveraging"};
private:
    template <Mode>
    uint32_t getNumberOfSignalsForAveraging(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const NumberOfSignalsForAveraging &numberOfSignalsForAveraging);

