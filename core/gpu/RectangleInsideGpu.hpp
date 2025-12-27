/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "ConfigReader.hpp"
#include "ElementInsideGpu.hpp"
#include <glad/glad.h>

enum class RectangleType
{
    BAR,
    TRANSPARENT_BAR,
    BACKGROUND
};

template<RectangleType rectangleType>
class RectangleInsideGpu : public ElementInsideGpu
{
public:
    RectangleInsideGpu(const Rectangle &rectangle);
    RectangleInsideGpu(const Rectangle &rectangle, const ColorsOfRectanglePerVertices &colorsOfRectangle);
    void move(const float y);
    void draw();
    static void updateTime(const float timeInMilliSeconds);
    static void updateBoundary(const float xBegin, const float xEnd);
    static void updateThemeNumber(const uint16_t themeNumber);
    static void initialize(const char *fsConfig = getDefaultFragmentShader());
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
    static GLuint timeLoc;
    static GLuint boundaryLoc;
    static GLuint themeNumberLoc;
};
