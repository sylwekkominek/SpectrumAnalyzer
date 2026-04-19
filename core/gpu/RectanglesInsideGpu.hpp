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
    SECONDARY_BAR,
    BACKGROUND
};

template<RectangleType rectangleType>
class RectanglesInsideGpu : public ElementInsideGpu
{
public:

    RectanglesInsideGpu(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle={{0,{1,1,1,1}}, {1,{1,1,1,1}},{2,{1,1,1,1}},{3,{1,1,1,1}}});

    void move(const std::vector<float> &positionsInPercents);
    void draw();
    static void updateTime(const float timeInMilliSeconds);
    static void updateBoundary(const float xBegin, const float xEnd);
    static void initialize(const char *fsConfig = getDefaultFragmentShader());
    static void finalize();
private:
    float percentToPositon(float percent);
    static const char* getVertexShader();

    struct Instance
    {
        float x, y;
    };

    struct Vertex
    {
        float x, y;
        float r, g, b, t;
    };

    GLuint vao;
    GLuint vertexBuffer;
    GLuint instancesBuffer;

    const GLuint ATTR_POS = 0u;
    const GLuint ATTR_COLOR = 1u;
    const GLuint ATTR_OFFSET = 2u;

    std::vector<Instance> instances;

    static GLuint vs;
    static GLuint fs;

    static GLuint pipeline;
    static GLuint timeLoc;
    static GLuint boundaryLoc;

    static constexpr GLuint BINDING_VERTEX = 0;
    static constexpr GLuint BINDING_INSTANCE = 1;
};
