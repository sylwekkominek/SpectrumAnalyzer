/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "ElementInsideGpu.hpp"
#include <glad/glad.h>
#include <vector>

class LinesInsideGpu : public ElementInsideGpu
{
public:
    LinesInsideGpu(const uint32_t numberOfLines, const std::vector<float> &color={1.0, 1.0,1.0,1.0});
    void draw(const Lines &lines);
    static void initialize();
    static void finalize();
private:
    float percentToPositon(float percent);
    static const char* getVertexShader();
    static const char* getFragmentShader();

    struct Instance
    {
        float x0, y0;
        float x1, y1;
    };

    struct Vertex
    {
        float r, g, b,t;
    };

    GLuint vao;
    GLuint colorBuffer;
    GLuint instanceBuffer;

    std::vector<Instance> instances;

    const GLuint ATTR_COLOR = 0u;
    const GLuint ATTR_P0    = 1u;
    const GLuint ATTR_P1    = 2u;

    static GLuint vs;
    static GLuint fs;
    static GLuint pipeline;

    static constexpr GLuint BINDING_COLOR    = 0;
    static constexpr GLuint BINDING_INSTANCE = 1;
};

