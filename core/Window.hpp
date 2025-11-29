/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "WindowBase.hpp"
#include "gpu/Gpu.hpp"
#include <vector>

class Window : public WindowBase
{
public:
    Window(const Configuration &config, const bool fullScreenEnabled);
    void initializeGPU();
    void draw(const std::vector<float> &data);
    ~Window();

private:

    Gpu gpu;
    AnyData anyData;
    std::vector<std::pair<std::string, std::function<void()>>> operations;

};
