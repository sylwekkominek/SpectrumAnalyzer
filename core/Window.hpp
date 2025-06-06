/*
 * Copyright (C) 2024-2025, Sylwester Kominek
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


struct WindowConfig
{
    uint16_t horizontalSize;
    uint16_t verticalSize;
    uint16_t numberOfRectangles;
    double gapWidthInRelationToRectangleWidth;
    float smallRectangleHeightInPercentOfScreenSize;
    ColorsOfRectanglePerVertices colorsOfRectangle;
    ColorsOfRectanglePerVertices colorsOfSmallRectangle;
    std::string advancedColorSetting;
};

class Window
{
public:
    Window(const WindowConfig &windowConfig);
    void initializeGPU();
    void draw(const std::vector<float> &positions, const std::vector<float> &positionsOfSmallRectangles={});
    bool checkIfWindowShouldBeClosed();
    bool checkIfWindowShouldBeRecreated();
    ~Window();

private:

    struct Rectangle
    {
        float xBegin;
        float xEnd;
        float yBegin;
        float yEnd;
    };

    class RectangleInsideGpu
    {
    public:
        RectangleInsideGpu(const Rectangle &rectangle);
        RectangleInsideGpu(const Rectangle &rectangle, const ColorsOfRectanglePerVertices &colorsOfRectangle);
        void draw(GLuint vs, float x, float y);
        ~RectangleInsideGpu();

    private:

        const uint indexOfRed = 0;
        const uint indexOfGreen = 1;
        const uint indexOfBlue = 2;

        GLuint vao;
        GLuint vertexBuffer;
        GLuint colorBuffer;

        const GLuint ATTR_POS = 0u;
        const GLuint ATTR_COLOR = 1u;
    };

    std::vector<Rectangle> rectanglesFactory(const float heightInPercentOfScreenSize);

    void initializeRectangles(std::vector<RectangleInsideGpu> &rectanglesInsideGpu, const std::vector<Rectangle> &rectangles);
    void initializeRectangles(std::vector<RectangleInsideGpu> &rectanglesInsideGpu, const std::vector<Rectangle> &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle);
    float percentToPositon(float percent);
    float percentToPositonSmallElement(float percent);

    const char* getVertexShader();
    const char* getFragmentShader();

    void prepareShaders(const char *vsConfig, const char *fsConfig);
    GLuint compileShader(const GLchar* source, GLenum stage, const std::string& msg);

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void windowMaximizeCallback(GLFWwindow* window, int maximized);


    const WindowConfig windowConfig;

    std::vector<RectangleInsideGpu> rectanglesInsideGpu;
    std::vector<RectangleInsideGpu> smallRectanglesInsideGpu;


    GLuint vs;
    GLuint fs;
    GLuint pipeline;
    GLFWwindow* window;
    static bool isMaximized;
};

using namespace std::chrono;

class SmallRectanglesPositionCalculator
{
public:
    SmallRectanglesPositionCalculator(uint numberOfRectangles, float speedOfFalling = 600, bool accelerationStateOfFalling=true, float initialPositionInPercents=0);
    std::vector<float> getPositions(const std::vector<float> &dataToBePrinted);
private:

    const uint numberOfRectangles;
    const float speedOfFalling;
    const bool accelerationStateOfFalling;
    std::vector<time_point<high_resolution_clock>> previousTimes;
    std::vector<float> dataToBePrintedWithSmallElements;
};



