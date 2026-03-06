/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <ostream>
#include <cstdint>

struct NumberOfRectangles
{
    NumberOfRectangles(uint16_t value);
    std::string getInfo();
    uint16_t value;
};

std::ostream& operator<<(std::ostream& os, const NumberOfRectangles &numberOfRectangles);
