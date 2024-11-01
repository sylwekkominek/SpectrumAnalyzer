/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "ConfigReader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <chrono>


class Window
{
public:

    Window(uint16_t horizontalSize, uint16_t verticalSize, const uint16_t numberOFRectangles, const ColorsOrRectanglePerVertices &colorsOfRectangle, const ColorsOrRectanglePerVertices &colorsOfSmallRectangle);
    void initializeGPU();
    void draw(const std::vector<float> &positions, const std::vector<float> &positionsOfSmallRectangles={});
    bool checkIfWindowShouldBeClosed();
    ~Window();

private:

    class Rectangle
    {

    public:
        Rectangle(float xBegin, float xEnd, float yBegin,float yEnd, const ColorsOrRectanglePerVertices &colorsOfRectangle);
        void updateGpu();
        void draw(GLuint vs, float x, float y);
        ~Rectangle();

    private:

        const uint indexOfRed = 0;
        const uint indexOfGreen = 1;
        const uint indexOfBlue = 2;
        static constexpr uint32_t sizeOfVerticesTable{30};

        GLuint vao;
        GLuint vertexBuffer;

        float vertices[sizeOfVerticesTable]={};
        const GLuint ATTR_POS = 0u;
        const GLuint ATTR_COLOR = 1u;
    };


    void initializeRectangles(std::vector<Rectangle> &rectangles, float yBegin,float yEnd, const ColorsOrRectanglePerVertices &colorsOfRectangle);
    float percentToPositon(float percent);
    float percentToPositonSmallElement(float percent);
    GLuint compileShader(const GLchar* source, GLenum stage, const std::string& msg);

    const uint16_t numberOFRectangles;
    const ColorsOrRectanglePerVertices colorsOfRectangle;
    const ColorsOrRectanglePerVertices colorsOfSmallRectangle;
    std::vector<Rectangle> rectangles;
    std::vector<Rectangle> smallRectangles;

    GLuint vs;
    GLuint fs;
    GLuint pipeline;
    GLFWwindow* window;


    const char* a = R"(#version 330 core
#extension GL_ARB_explicit_uniform_location : require
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec3 inColor;

layout (location = 0) uniform vec2 posOffset;
out vec3 vertColor;

void main()
{
    gl_Position = vec4(inPos+posOffset, 0.f, 1.f);
    vertColor = inColor;
})";

    const char* b = R"(#version 330 core

in vec3 vertColor;
out vec4 Color;

void main()
{
    Color = vec4(vertColor, 1.f);
})";

};

using namespace std::chrono;

class SmallRectanglesPositionCalculator
{
public:
    SmallRectanglesPositionCalculator(uint numberOfRectangles, float speedOfFalling = 600, bool accelerationStateOfFalling=true, float initialPositionInPercents=100);
    std::vector<float> getPositions(const std::vector<float> &dataToBePrinted);
private:

    const uint numberOfRectangles;
    const float speedOfFalling;
    const bool accelerationStateOfFalling;
    std::vector<time_point<high_resolution_clock>> previousTimes;
    std::vector<float> dataToBePrintedWithSmallElements;
};



