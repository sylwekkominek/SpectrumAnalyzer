/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <vector>
#include <ostream>

struct ScalingFactor
{
    ScalingFactor(float value);
    ScalingFactor(const std::vector<float> &window);
    float getScalingFactor(const std::vector<float> &window);
    std::string getInfo();
    float value;
    const std::string name{"ScalingFactor"};
};

std::ostream& operator<<(std::ostream& os, const ScalingFactor &scalingFactor);
