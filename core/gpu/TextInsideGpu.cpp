/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "TextInsideGpu.hpp"
#include <glad/glad.h>
#define GLT_IMPLEMENTATION
#include "../glText/gltext.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>

class TextInsideGpu::TextInsideGpuImpl
{
public:
    TextInsideGpuImpl(const std::string &str, const std::vector<float> &color);
    void draw(const uint16_t horizontalAligment, const float x, const float y);
    void draw(const std::string &str, const uint16_t horizontalAligment, const float x, const float y);
    static void initialize();
    static void finalize();
    ~TextInsideGpuImpl() = default;
private:
    const std::vector<float> color;
    std::unique_ptr<GLTtext, void(*)(GLTtext*)> textPtr;
};


void TextInsideGpu::TextInsideGpuImpl::initialize()
{
    gltInit();
}

TextInsideGpu::TextInsideGpuImpl::TextInsideGpuImpl(const std::string &str, const std::vector<float> &color) :textPtr(gltCreateText(), gltDeleteText), color(color)
{
    gltSetText(textPtr.get(), str.c_str());
}

void TextInsideGpu::TextInsideGpuImpl::draw(const uint16_t horizontalAligment, const float x, const float y)
{
    gltBeginDraw();
    gltColor(color.at(0), color.at(1), color.at(2), color.at(3));

    gltDrawText2DAligned(textPtr.get(),
                         (GLfloat)(x),
                         (GLfloat)(y),
                         1.0f,
                         horizontalAligment, GLT_CENTER);

    gltEndDraw();
    glUseProgram(0);
}

void TextInsideGpu::TextInsideGpuImpl::draw(const std::string &str,const uint16_t horizontalAligment, const float x, const float y)
{
    gltSetText(textPtr.get(), str.c_str());
    gltBeginDraw();
    gltColor(color.at(0), color.at(1), color.at(2), color.at(3));

    gltDrawText2DAligned(textPtr.get(),
                         (GLfloat)(x),
                         (GLfloat)(y),
                         1.0f,
                         horizontalAligment, GLT_BOTTOM);

    gltEndDraw();
    glUseProgram(0);
}

void TextInsideGpu::TextInsideGpuImpl::finalize()
{
    gltTerminate();
}

TextInsideGpu::TextInsideGpu() : textInsideGpuImpl(std::make_unique<TextInsideGpuImpl>(std::string(""), std::vector<float>{1.0, 1.0,1.0,1.0}))
{
}

TextInsideGpu::TextInsideGpu(const std::string &str, const std::vector<float> &color) : textInsideGpuImpl(std::make_unique<TextInsideGpuImpl>(str, color))
{
}

void TextInsideGpu::initialize()
{
    TextInsideGpuImpl::initialize();
}

void TextInsideGpu::draw(const HorizontalAligment horizontalAligment, const float x, const float y)
{
    textInsideGpuImpl->draw((uint16_t)horizontalAligment, x, y);
}

void TextInsideGpu::draw(const std::string &str, const HorizontalAligment horizontalAligment, const float x, const float y)
{
    textInsideGpuImpl->draw(str,(uint16_t)horizontalAligment, x, y);
}

void TextInsideGpu::finalize()
{
    TextInsideGpuImpl::finalize();
}

float TextInsideGpu::percentToPositon(float percent)
{
    return (100-percent)/100;
}


TextInsideGpu::~TextInsideGpu() = default;
TextInsideGpu::TextInsideGpu(TextInsideGpu&&) = default;
TextInsideGpu& TextInsideGpu::operator=(TextInsideGpu&&) = default;


