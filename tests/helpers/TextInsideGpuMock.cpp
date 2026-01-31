/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "TextInsideGpuMock.hpp"
#include <functional>

std::function<void()> initializeFunction;
std::function<void(const HorizontalAligment, const VerticalAligment, const float, const float)> drawFunction;
std::function<void(const std::string &, const HorizontalAligment, const VerticalAligment, const float, const float)> draw2Function;
std::function<void(const HorizontalAligment, const VerticalAligment)> draw3Function;
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

void TextInsideGpu::draw(const HorizontalAligment horizontalAligment, const VerticalAligment verticalAligment, const float x, const float y)
{
    drawFunction(horizontalAligment, verticalAligment, x, y);
}

void TextInsideGpu::draw(const std::string &str, const HorizontalAligment horizontalAligment, const VerticalAligment verticalAligment, const float x, const float y)
{
    draw2Function(str, horizontalAligment, verticalAligment, x, y);
}

void TextInsideGpu::draw(const HorizontalAligment horizontalAligment, const VerticalAligment verticalAligment)
{
    draw3Function(horizontalAligment, verticalAligment);
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

    drawFunction = [this](const HorizontalAligment horizontalAligment, const VerticalAligment verticalAligment, const float x, const float y)
    {
        this->draw(horizontalAligment, verticalAligment, x, y);
    };

    draw2Function = [this](const std::string &str, const HorizontalAligment horizontalAligment, const VerticalAligment verticalAligment,const float x, const float y)
    {
        this->draw(str, horizontalAligment, verticalAligment, x, y);
    };

    draw3Function = [this](const HorizontalAligment horizontalAligment, const VerticalAligment verticalAligment)
    {
        this->draw(horizontalAligment, verticalAligment);
    };

    finalizeFunction = [this]()
    {
        this->finalize();
    };
}
