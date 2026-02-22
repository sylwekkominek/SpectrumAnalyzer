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

struct DesiredFrameRate
{
    DesiredFrameRate(uint32_t value);
    DesiredFrameRate(const ThemeConfig themeConfig);
    std::string getInfo();
    uint32_t value;
    const std::string name{"DesiredFrameRate"};
private:
    uint32_t getDesiredFrameRate(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const DesiredFrameRate &desiredFrameRate);

