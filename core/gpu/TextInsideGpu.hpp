/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

enum class HorizontalAligment : uint16_t
{
    LEFT =0,
    CENTER = 1,
    RIGHT =2,
};

enum class VerticalAligment : uint16_t
{
    TOP =0,
    CENTER = 1,
    BOTTOM =2,
};

class TextInsideGpu
{
public:
    TextInsideGpu(const std::string &str, const std::vector<float> &color={1.0, 1.0,1.0,1.0});
    TextInsideGpu(const std::string &str, const float x, const float y, const std::vector<float> &color={1.0, 1.0,1.0,1.0});
    TextInsideGpu(TextInsideGpu&&);
    TextInsideGpu& operator=(TextInsideGpu&&);
    TextInsideGpu();
    ~TextInsideGpu();

    void draw(const HorizontalAligment horizontalAligment, const VerticalAligment verticalAligment);
    void draw(const HorizontalAligment horizontalAligment, const VerticalAligment verticalAligment, const float x, const float y);
    void draw(const std::string &str, const HorizontalAligment horizontalAligment, const VerticalAligment verticalAligment, const float x, const float y);

    static void initialize();
    static void finalize();

private:
    class TextInsideGpuImpl;
    std::unique_ptr<TextInsideGpuImpl> textInsideGpuImpl;
    float x{};
    float y{};

    float percentToPositon(float percent);
};



