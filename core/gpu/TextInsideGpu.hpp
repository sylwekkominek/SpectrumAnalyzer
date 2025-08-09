/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>


class TextInsideGpu
{
public:
    TextInsideGpu(const std::string &str, const std::vector<float> &color={1.0, 1.0,1.0,1.0});
    TextInsideGpu();
    ~TextInsideGpu();

    TextInsideGpu(TextInsideGpu&&);
    TextInsideGpu& operator=(TextInsideGpu&&);

    void draw(const float y);
    static void initialize();
    static void updateWindowSize(const float w, const float h);
    static void finalize();

private:
    class TextInsideGpuImpl;
    std::unique_ptr<TextInsideGpuImpl> textInsideGpuImpl;

    float percentToPositon(float percent);
};



