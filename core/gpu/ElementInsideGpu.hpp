/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

struct Point
{
    float x;
    float y;
};

using Line = std::vector<Point>;
using Lines = std::vector<Line>;
using Rectangle = std::vector<Point>;
using Rectangles = std::vector<Rectangle>;


class ElementInsideGpu
{
public:
    static void prepareShaders(GLuint &pipeline, GLuint &vs, GLuint &fs, const char * vsConfig, const char *fsConfig);
protected:
    static void removeShaders(GLuint &pipeline, GLuint &vs, GLuint &fs);
    static const char* getDefaultFragmentShader();

    const uint32_t indexOfRed = 0;
    const uint32_t indexOfGreen = 1;
    const uint32_t indexOfBlue = 2;
    const uint32_t indexOfTransparency = 3;

private:
    static GLuint compileShader(const GLchar* source, GLenum stage, const std::string& msg);
};
