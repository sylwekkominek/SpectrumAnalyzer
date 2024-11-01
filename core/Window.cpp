/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Window.hpp"
#include <iostream>

Window::Rectangle::Rectangle(float xBegin, float xEnd, float yBegin,float yEnd, const ColorsOrRectanglePerVertices &colorsOfRectangle)
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

        const float tempVertices[sizeOfVerticesTable]={
                xEnd, yEnd, colorsOfRectangle.at(2).at(indexOfRed), colorsOfRectangle.at(2).at(indexOfGreen),colorsOfRectangle.at(2).at(indexOfBlue),     //2
                xEnd, yBegin, colorsOfRectangle.at(1).at(indexOfRed), colorsOfRectangle.at(1).at(indexOfGreen),colorsOfRectangle.at(1).at(indexOfBlue),   //1
                xBegin, yBegin, colorsOfRectangle.at(0).at(indexOfRed), colorsOfRectangle.at(0).at(indexOfGreen),colorsOfRectangle.at(0).at(indexOfBlue), //0
                xEnd, yEnd, colorsOfRectangle.at(2).at(indexOfRed), colorsOfRectangle.at(2).at(indexOfGreen),colorsOfRectangle.at(2).at(indexOfBlue),     //5
                xBegin, yBegin, colorsOfRectangle.at(0).at(indexOfRed), colorsOfRectangle.at(0).at(indexOfGreen),colorsOfRectangle.at(0).at(indexOfBlue), //4
                xBegin, yEnd, colorsOfRectangle.at(3).at(indexOfRed), colorsOfRectangle.at(3).at(indexOfGreen),colorsOfRectangle.at(3).at(indexOfBlue),   //3
            };

        for(uint i=0;i<sizeOfVerticesTable;++i)
        {
            vertices[i] = tempVertices[i];
        }
}

void Window::Rectangle::updateGpu()
{
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vertexBuffer);
    glNamedBufferStorage(vertexBuffer, sizeof(vertices), vertices, 0);

    glEnableVertexArrayAttrib(vao, ATTR_POS);
    glVertexArrayAttribFormat(vao, ATTR_POS, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(vao, ATTR_POS, vertexBuffer, 0, 5*sizeof(float));
    glVertexArrayAttribBinding(vao, ATTR_POS, ATTR_POS);

    glEnableVertexArrayAttrib(vao, ATTR_COLOR);
    glVertexArrayAttribFormat(vao, ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(vao, ATTR_COLOR, vertexBuffer, 2*sizeof(float), 5 * sizeof(float));
    glVertexArrayAttribBinding(vao, ATTR_COLOR, ATTR_COLOR);
}
void Window::Rectangle::draw(GLuint vs, float x, float y)
{
    glBindVertexArray(vao);
    glProgramUniform2f(vs, 0, x, y);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

Window::Rectangle::~Rectangle()
{
}

Window::Window(uint16_t horizontalSize, uint16_t verticalSize, const uint16_t numberOFRectangles, const ColorsOrRectanglePerVertices &colorsOfRectangle, const ColorsOrRectanglePerVertices &colorsOfSmallRectangle) :
    numberOFRectangles(numberOFRectangles),
    colorsOfRectangle(colorsOfRectangle),
    colorsOfSmallRectangle(colorsOfSmallRectangle)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    window = glfwCreateWindow(horizontalSize, verticalSize, "Audio spectrum analyzer implemented by Sylwester Kominek", glfwGetPrimaryMonitor(), nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGL();
}

void Window::initializeGPU()
{
    vs = compileShader(a, GL_VERTEX_SHADER, "VS log");
    fs = compileShader(b, GL_FRAGMENT_SHADER, "VS log");

    glCreateProgramPipelines(1, &pipeline);
    glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vs);
    glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fs);

    const float yBeginOfBigElement = -1;
    const float yEndOfBigElement = 1;

    const float yBeginOfSmallElement = -0.01;
    const float yEndOfSmallElement = 0.01;

    initializeRectangles(rectangles, yBeginOfBigElement,yEndOfBigElement, colorsOfRectangle);
    initializeRectangles(smallRectangles, yBeginOfSmallElement,yEndOfSmallElement, colorsOfSmallRectangle);

    glBindProgramPipeline(pipeline);
}

void Window::draw(const std::vector<float> &positions, const std::vector<float> &positionsOfSmallRectangles)
{
    glClear(GL_COLOR_BUFFER_BIT);

    for(uint i=0;i<positions.size();++i)
    {
        rectangles.at(i).draw(vs,0, percentToPositon( positions.at(i)));
    }

    for(uint i=0;i<positionsOfSmallRectangles.size();++i)
    {
        smallRectangles.at(i).draw(vs,0, percentToPositonSmallElement(positionsOfSmallRectangles.at(i)));
    }

    glfwSwapBuffers(window);
}

bool Window::checkIfWindowShouldBeClosed()
{
    glfwPollEvents();
    return glfwWindowShouldClose(window);
}

Window::~Window()
{
   glDeleteProgramPipelines(1, &pipeline);
   glDeleteProgram(vs);
   glDeleteProgram(fs);
   glfwTerminate();
}

void Window::initializeRectangles(std::vector<Rectangle> &rectangles, float yBegin,float yEnd, const ColorsOrRectanglePerVertices &colorsOfRectangle)
{
    const float fullScreenSize = 2.0;
    const float xBeginOfZeroElement = -1;

    for(int i=0;i<numberOFRectangles;++i)
    { 
        float xWidth =  (fullScreenSize/numberOFRectangles);
        float xBegin =  xBeginOfZeroElement + i*xWidth;
        float xEnd = xBegin + xWidth;

        Rectangle rectangle(xBegin,xEnd,yBegin,yEnd, colorsOfRectangle);
        rectangle.updateGpu();
        rectangles.emplace_back(std::move(rectangle));
    }
}

float Window::percentToPositon(float percent)
{
    return (percent/50) -2;
}

float Window::percentToPositonSmallElement(float percent)
{
    return (percent/50) -1.01;
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




