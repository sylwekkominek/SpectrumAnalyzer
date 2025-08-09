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
#include <atomic>



class TextInsideGpu::TextInsideGpuImpl
{
public:
    TextInsideGpuImpl(const std::string &str, const std::vector<float> &color);
    void draw(const float y);
    static void initialize();
    static void updateWindowSize(const float w, const float h);
    static void finalize();
    ~TextInsideGpuImpl() = default;
private:
    const std::vector<float> color;
    static std::atomic<float> width;
    static std::atomic<float> height;
    std::unique_ptr<GLTtext, void(*)(GLTtext*)> textPtr;
};

std::atomic<float> TextInsideGpu::TextInsideGpuImpl::width=0;
std::atomic<float> TextInsideGpu::TextInsideGpuImpl::height=0;


void TextInsideGpu::TextInsideGpuImpl::initialize()
{
    gltInit();
}

TextInsideGpu::TextInsideGpuImpl::TextInsideGpuImpl(const std::string &str, const std::vector<float> &color) :textPtr(gltCreateText(), gltDeleteText), color(color)
{
    gltSetText(textPtr.get(), str.c_str());
}

void TextInsideGpu::TextInsideGpuImpl::draw(const float y)
{
    gltBeginDraw();
    gltColor(color.at(0), color.at(1), color.at(2), color.at(3));

    gltDrawText2DAligned(textPtr.get(),
                         (GLfloat)(0),
                         (GLfloat)(y*height),
                         1.0f,
                         GLT_LEFT, GLT_CENTER);
    gltDrawText2DAligned(textPtr.get(),
                         (GLfloat)(width),
                         (GLfloat)(y*height),
                         1.0f,
                         GLT_RIGHT, GLT_CENTER);

    gltEndDraw();
    glUseProgram(0);
}

void TextInsideGpu::TextInsideGpuImpl::updateWindowSize(const float w, const float h)
{
    width = w;
    height = h;
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

void TextInsideGpu::draw(const float y)
{
    textInsideGpuImpl->draw(percentToPositon(y));
}

void TextInsideGpu::updateWindowSize(const float w, const float h)
{
    TextInsideGpuImpl::updateWindowSize(w,h);
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


