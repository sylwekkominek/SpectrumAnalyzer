/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "ConfigReader.hpp"
#include <glad/glad.h>
#include <string>
#include <vector>

struct Point
{
    float x;
    float y;
};

using Line = std::vector<Point>;
using Rectangle = std::vector<Point>;

class ElementInsideGpu
{
public:
    static void prepareShaders(GLuint &pipeline, GLuint &vs, GLuint &fs, const char * vsConfig, const char *fsConfig);
protected:
    static void removeShaders(GLuint &pipeline, GLuint &vs, GLuint &fs);

    const uint indexOfRed = 0;
    const uint indexOfGreen = 1;
    const uint indexOfBlue = 2;
    const uint indexOfTransparency = 3;

private:
    static GLuint compileShader(const GLchar* source, GLenum stage, const std::string& msg);
};

class RectangleInsideGpu : public ElementInsideGpu
{
public:
    RectangleInsideGpu(const Rectangle &rectangle);
    RectangleInsideGpu(const Rectangle &rectangle, const ColorsOfRectanglePerVertices &colorsOfRectangle);
    void move(const float y);
    static void initialize(const char *fsConfig);
    static void finalize();
private:
    float percentToPositon(float percent);
    static const char* getVertexShader();

    GLuint vao;
    GLuint vertexBuffer;
    GLuint colorBuffer;

    const GLuint ATTR_POS = 0u;
    const GLuint ATTR_COLOR = 1u;

    static GLuint vs;
    static GLuint fs;
    static GLuint pipeline;
};

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

