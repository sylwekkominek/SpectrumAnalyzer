/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "ElementInsideGpu.hpp"
#include <glad/glad.h>
#include <vector>

class LineInsideGpu : public ElementInsideGpu
{
public:
    LineInsideGpu();
    void draw(const Line &line, const std::vector<float> &color={1.0, 1.0,1.0,1.0});
    static void initialize();
    static void finalize();
private:
    float percentToPositon(float percent);
    static const char* getVertexShader();
    static const char* getFragmentShader();
    GLuint vao;
    static bool isInitialized;
    static GLuint vs;
    static GLuint fs;
    static GLint p0Loc;
    static GLint p1Loc;
    static GLint colorLoc;
    static GLuint pipeline;
};

