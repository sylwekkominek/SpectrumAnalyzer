/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Window.hpp"
#include <algorithm>
#include <iostream>

bool Window::isMaximized = false;

Window::RectangleInsideGpu::RectangleInsideGpu(const Rectangle &rectangle)
{
    /*
        Each rectangle consists of 2 triangles

        3______5   2
        |     /   /|
        |    /   / |
        |   /   /  |
        |  /   /   |
        | /   /    |
        |/   /_____|
        4   0      1
    */

        const float vertices[]={
                rectangle.xEnd, rectangle.yEnd,     //2
                rectangle.xEnd, rectangle.yBegin,   //1
                rectangle.xBegin, rectangle.yBegin, //0
                rectangle.xEnd, rectangle.yEnd,     //5
                rectangle.xBegin, rectangle.yBegin, //4
                rectangle.xBegin, rectangle.yEnd,   //3
            };

        glCreateVertexArrays(1, &vao);

        glCreateBuffers(1, &vertexBuffer);
        glNamedBufferStorage(vertexBuffer, sizeof(vertices), vertices, 0);

        glEnableVertexArrayAttrib(vao, ATTR_POS);
        glVertexArrayAttribFormat(vao, ATTR_POS, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(vao, ATTR_POS, vertexBuffer, 0, 2*sizeof(float));
        glVertexArrayAttribBinding(vao, ATTR_POS, ATTR_POS);
}

Window::RectangleInsideGpu::RectangleInsideGpu(const Rectangle &rectangle, const ColorsOfRectanglePerVertices &colorsOfRectangle):
    RectangleInsideGpu(rectangle)
{

    const float colors[]={
        colorsOfRectangle.at(2).at(indexOfRed), colorsOfRectangle.at(2).at(indexOfGreen),colorsOfRectangle.at(2).at(indexOfBlue),     //2
        colorsOfRectangle.at(1).at(indexOfRed), colorsOfRectangle.at(1).at(indexOfGreen),colorsOfRectangle.at(1).at(indexOfBlue),     //1
        colorsOfRectangle.at(0).at(indexOfRed), colorsOfRectangle.at(0).at(indexOfGreen),colorsOfRectangle.at(0).at(indexOfBlue),     //0
        colorsOfRectangle.at(2).at(indexOfRed), colorsOfRectangle.at(2).at(indexOfGreen),colorsOfRectangle.at(2).at(indexOfBlue),     //5
        colorsOfRectangle.at(0).at(indexOfRed), colorsOfRectangle.at(0).at(indexOfGreen),colorsOfRectangle.at(0).at(indexOfBlue),     //4
        colorsOfRectangle.at(3).at(indexOfRed), colorsOfRectangle.at(3).at(indexOfGreen),colorsOfRectangle.at(3).at(indexOfBlue),     //3
    };

    glCreateBuffers(1, &colorBuffer);
    glNamedBufferStorage(colorBuffer, sizeof(colors), colors, 0);

    glEnableVertexArrayAttrib(vao, ATTR_COLOR);
    glVertexArrayAttribFormat(vao, ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(vao, ATTR_COLOR, colorBuffer, 0, 3 * sizeof(float));
    glVertexArrayAttribBinding(vao, ATTR_COLOR, ATTR_COLOR);
}

void Window::RectangleInsideGpu::draw(GLuint vs, float x, float y)
{
    glBindVertexArray(vao);
    glProgramUniform2f(vs, 0, x, y);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

Window::RectangleInsideGpu::~RectangleInsideGpu()
{
}

Window::Window(const WindowConfig &windowConfig) :
    windowConfig(windowConfig)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    window = glfwCreateWindow(windowConfig.horizontalSize, windowConfig.verticalSize, "SpectrumAnalyzer", nullptr, nullptr);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetWindowMaximizeCallback(window, windowMaximizeCallback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGL();
}

void Window::initializeGPU()
{
    const float fullScreenSizeInPercents{100};

    prepareShaders(getVertexShader(), getFragmentShader());
    initializeRectangles(rectanglesInsideGpu, rectanglesFactory(fullScreenSizeInPercents), windowConfig.colorsOfRectangle);
    initializeRectangles(smallRectanglesInsideGpu, rectanglesFactory(windowConfig.smallRectangleHeightInPercentOfScreenSize), windowConfig.colorsOfSmallRectangle);

    glBindProgramPipeline(pipeline);
}

void Window::draw(const std::vector<float> &positions, const std::vector<float> &positionsOfSmallRectangles)
{
    glClear(GL_COLOR_BUFFER_BIT);

    for(uint i=0;i<positions.size();++i)
    {
        rectanglesInsideGpu.at(i).draw(vs,0, percentToPositon( positions.at(i)));
    }

    for(uint i=0;i<positionsOfSmallRectangles.size();++i)
    {
        smallRectanglesInsideGpu.at(i).draw(vs,0, percentToPositonSmallElement(positionsOfSmallRectangles.at(i)));
    }

    glfwSwapBuffers(window);
}

bool Window::checkIfWindowShouldBeClosed()
{
    glfwPollEvents();

    if (glfwWindowShouldClose(window))
    {
        return true;
    }

    return false;
}

bool Window::checkIfWindowShouldBeRecreated()
{
    glfwPollEvents();

    return ((isMaximized && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))  ? (isMaximized = false, true) : false;
}

void Window::framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::windowMaximizeCallback(GLFWwindow* glfwWindow, int maximized)
{
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwSetWindowMonitor(glfwWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    isMaximized = (maximized == GLFW_TRUE);
}

Window::~Window()
{
   glDeleteProgramPipelines(1, &pipeline);
   glDeleteProgram(vs);
   glDeleteProgram(fs);
   glfwDestroyWindow(window);
}

std::vector<Window::Rectangle> Window::rectanglesFactory(const float heightInPercentOfScreenSize)
{
    const double fullScreenSize = 2.0;
    const double xBeginOfZeroElement = -1;
    const double numberOfGaps = windowConfig.numberOfRectangles -1;
    const double xWidth =  (fullScreenSize/(windowConfig.numberOfRectangles + numberOfGaps * windowConfig.gapWidthInRelationToRectangleWidth));

    const float yBegin= -heightInPercentOfScreenSize/100;
    const float yEnd = heightInPercentOfScreenSize/100;

    std::vector<Rectangle> rectangles;
    rectangles.reserve(windowConfig.numberOfRectangles);

    for(uint i=0;i<windowConfig.numberOfRectangles;++i)
    {
        double xBegin =  xBeginOfZeroElement + i*xWidth*(1.0 + windowConfig.gapWidthInRelationToRectangleWidth);
        double xEnd = xBegin + xWidth;

        rectangles.emplace_back(Rectangle{static_cast<float>(xBegin),static_cast<float>(xEnd),static_cast<float>(yBegin),static_cast<float>(yEnd)});
    }

    return rectangles;
}

void Window::initializeRectangles(std::vector<RectangleInsideGpu> &rectanglesInsideGpu, const std::vector<Rectangle> &rectangles)
{
    for(const auto & rectangle : rectangles)
    {
        rectanglesInsideGpu.emplace_back(RectangleInsideGpu(rectangle));
    }
}

void Window::initializeRectangles(std::vector<RectangleInsideGpu> &rectanglesInsideGpu, const std::vector<Rectangle> &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle)
{
    for(const auto & rectangle : rectangles)
    {
        rectanglesInsideGpu.emplace_back(RectangleInsideGpu(rectangle, colorsOfRectangle));
    }
}

float Window::percentToPositon(float percent)
{
    return (std::min<float>(percent, 100)/50) -2;
}

float Window::percentToPositonSmallElement(float percent)
{
    return (std::min<float>(percent, 100)/50) -1.01;
}

const char* Window::getVertexShader()
{
    const char* shaderUsedWithColorsProvidedByUser = R"(#version 330 core
    #extension GL_ARB_explicit_uniform_location : require
    layout (location = 0) in vec2 inPos;
    layout (location = 1) in vec3 inColor;


    layout (location = 0) uniform vec2 posOffset;
    out vec3 vertColor;
    out vec4 calculatedPosition;


    void main()
    {
        gl_Position = vec4(inPos+posOffset, 0.f, 1.f);
        calculatedPosition = vec4(inPos+posOffset, 0.f, 1.f);
        vertColor = inColor;
    })";

    return shaderUsedWithColorsProvidedByUser;

}

const char* Window::getFragmentShader()
{
    return windowConfig.advancedColorSetting.c_str();
}

void Window::prepareShaders(const char * vsConfig, const char *fsConfig)
{
    vs = compileShader(vsConfig, GL_VERTEX_SHADER, "VS log");
    fs = compileShader(fsConfig, GL_FRAGMENT_SHADER, "VS log");

    glCreateProgramPipelines(1, &pipeline);
    glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vs);
    glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fs);
}

GLuint Window::compileShader(const GLchar* source, GLenum stage, const std::string& msg)
{
    GLuint shdr = glCreateShaderProgramv(stage, 1, &source);

    std::string log;
    log.resize(1024);
    glGetProgramInfoLog(shdr, log.size(), nullptr, &log.front());
    std::cout<<msg.c_str()<<": "<<log.c_str()<<std::endl;
    return shdr;
}

SmallRectanglesPositionCalculator::SmallRectanglesPositionCalculator(uint numberOfRectangles, float speedOfFalling, bool accelerationStateOfFalling, float initialPositionInPercents):
    numberOfRectangles(numberOfRectangles),
    speedOfFalling(speedOfFalling),
    accelerationStateOfFalling(accelerationStateOfFalling),
    dataToBePrintedWithSmallElements(numberOfRectangles, initialPositionInPercents),
    previousTimes(numberOfRectangles)
{
    for(uint i=0;i<numberOfRectangles;++i)
    {
        previousTimes[i] = high_resolution_clock::now();
    }
}

std::vector<float> SmallRectanglesPositionCalculator::getPositions(const std::vector<float> &dataToBePrinted)
{
    auto time = high_resolution_clock::now();

    for(uint i=0;i<numberOfRectangles;++i)
    {
        uint32_t diffInTime = (duration_cast<milliseconds>(time - previousTimes[i])).count();

        float diffInPosition = diffInTime/speedOfFalling;
        float newPosition = (dataToBePrintedWithSmallElements[i] - diffInPosition);
        newPosition = newPosition > 0 ?  newPosition : 0;

        if(dataToBePrinted[i] > newPosition)
        {
            dataToBePrintedWithSmallElements[i] = dataToBePrinted[i];
            previousTimes[i] = high_resolution_clock::now();
        }
        else
        {
            dataToBePrintedWithSmallElements[i] =  newPosition;
            previousTimes[i] = accelerationStateOfFalling ?  previousTimes[i]: time;
        }
    }

    return dataToBePrintedWithSmallElements;
}




