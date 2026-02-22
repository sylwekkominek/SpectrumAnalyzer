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

struct NumberOfSignalsForMaxHold
{
    NumberOfSignalsForMaxHold(uint32_t value);
    NumberOfSignalsForMaxHold(const ThemeConfig themeConfig);
    std::string getInfo();
    uint32_t value;
    const std::string name{"NumberOfSignalsForMaxHold"};
private:
    uint32_t getNumberOfSignalsForMaxHold(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const NumberOfSignalsForMaxHold &numberOfSignalsForMaxHold);

