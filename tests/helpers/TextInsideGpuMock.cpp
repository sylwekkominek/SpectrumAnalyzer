/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "TextInsideGpuMock.hpp"
#include <functional>

std::function<void()> initializeFunction;
std::function<void(const float)> drawFunction;
std::function<void(const float, const float)> updateWindowSizeFunction;
std::function<void()> finalizeFunction;


class TextInsideGpu::TextInsideGpuImpl
{
public:
    TextInsideGpuImpl(const std::string &, const std::vector<float> &)
    {

    }

    ~TextInsideGpuImpl()
    {
    }
};

TextInsideGpu::TextInsideGpu()
{
}

TextInsideGpu::TextInsideGpu(const std::string &str, const std::vector<float> &color)
{
}

void TextInsideGpu::initialize()
{
    initializeFunction();
}

void TextInsideGpu::draw(const float y)
{
    drawFunction(y);
}

void TextInsideGpu::updateWindowSize(const float w, const float h)
{
    updateWindowSizeFunction(w,h);
}

void TextInsideGpu::finalize()
{
    finalizeFunction();
}

float TextInsideGpu::percentToPositon(float percent)
{
    return 0;
}


TextInsideGpu::~TextInsideGpu() = default;
TextInsideGpu::TextInsideGpu(TextInsideGpu&&) = default;
TextInsideGpu& TextInsideGpu::operator=(TextInsideGpu&&) = default;


TextInsideGpuMock::TextInsideGpuMock()
{
    initializeFunction = [this]()
    {
        this->initialize();
    };

    drawFunction = [this](const float y)
    {
        this->draw(y);
    };

    updateWindowSizeFunction = [this](const float w, const float h)
    {
        this->updateWindowSize(w,h);
    };

    finalizeFunction = [this]()
    {
        this->finalize();
    };
}
