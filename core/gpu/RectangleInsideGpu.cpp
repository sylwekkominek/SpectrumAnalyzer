/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "RectangleInsideGpu.hpp"

template<RectangleType rectangleType>
GLuint RectangleInsideGpu<rectangleType>::vs = 0;

template<RectangleType rectangleType>
GLuint RectangleInsideGpu<rectangleType>::fs = 0;

template<RectangleType rectangleType>
GLuint RectangleInsideGpu<rectangleType>::pipeline = 0;

template<RectangleType rectangleType>
GLuint RectangleInsideGpu<rectangleType>::timeLoc = 0;

template<RectangleType rectangleType>
GLuint RectangleInsideGpu<rectangleType>::boundaryLoc = 0;

template<RectangleType rectangleType>
GLuint RectangleInsideGpu<rectangleType>::themeNumberLoc = 0;


template<RectangleType rectangleType>
RectangleInsideGpu<rectangleType>::RectangleInsideGpu(const Rectangle &rectangle)
{
    glCreateVertexArrays(1, &vao);

    glCreateBuffers(1, &vertexBuffer);
    glNamedBufferStorage(vertexBuffer, rectangle.size()*sizeof(Rectangle::value_type), rectangle.data(), 0);

    glEnableVertexArrayAttrib(vao, ATTR_POS);
    glVertexArrayAttribFormat(vao, ATTR_POS, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(vao, ATTR_POS, vertexBuffer, 0, sizeof(Point));
    glVertexArrayAttribBinding(vao, ATTR_POS, ATTR_POS);
}

template<RectangleType rectangleType>
RectangleInsideGpu<rectangleType>::RectangleInsideGpu(const Rectangle &rectangle, const ColorsOfRectanglePerVertices &colorsOfRectangle):
    RectangleInsideGpu(rectangle)
{

    const float colors[]={
        colorsOfRectangle.at(0).at(indexOfRed), colorsOfRectangle.at(0).at(indexOfGreen),colorsOfRectangle.at(0).at(indexOfBlue),colorsOfRectangle.at(0).at(indexOfTransparency),    //0
        colorsOfRectangle.at(1).at(indexOfRed), colorsOfRectangle.at(1).at(indexOfGreen),colorsOfRectangle.at(1).at(indexOfBlue),colorsOfRectangle.at(1).at(indexOfTransparency),    //1
        colorsOfRectangle.at(2).at(indexOfRed), colorsOfRectangle.at(2).at(indexOfGreen),colorsOfRectangle.at(2).at(indexOfBlue),colorsOfRectangle.at(2).at(indexOfTransparency),    //2

        colorsOfRectangle.at(2).at(indexOfRed), colorsOfRectangle.at(2).at(indexOfGreen),colorsOfRectangle.at(2).at(indexOfBlue),colorsOfRectangle.at(2).at(indexOfTransparency),    //5
        colorsOfRectangle.at(0).at(indexOfRed), colorsOfRectangle.at(0).at(indexOfGreen),colorsOfRectangle.at(0).at(indexOfBlue),colorsOfRectangle.at(0).at(indexOfTransparency),    //4
        colorsOfRectangle.at(3).at(indexOfRed), colorsOfRectangle.at(3).at(indexOfGreen),colorsOfRectangle.at(3).at(indexOfBlue),colorsOfRectangle.at(3).at(indexOfTransparency),    //3
    };

    glCreateBuffers(1, &colorBuffer);
    glNamedBufferStorage(colorBuffer, sizeof(colors), colors, 0);

    glEnableVertexArrayAttrib(vao, ATTR_COLOR);
    glVertexArrayAttribFormat(vao, ATTR_COLOR, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(vao, ATTR_COLOR, colorBuffer, 0, 4 * sizeof(float));
    glVertexArrayAttribBinding(vao, ATTR_COLOR, ATTR_COLOR);
}

template<RectangleType rectangleType>
void RectangleInsideGpu<rectangleType>::initialize(const char *fsConfig)
{
    prepareShaders(pipeline, vs, fs, getVertexShader(),fsConfig);
    timeLoc = glGetUniformLocation(fs, "timeInMilliSeconds");
    boundaryLoc = glGetUniformLocation(fs, "boundary");
    themeNumberLoc = glGetUniformLocation(fs, "themeNumber");
}

template<RectangleType rectangleType>
void RectangleInsideGpu<rectangleType>::finalize()
{
    ElementInsideGpu::removeShaders(pipeline, vs,fs);
}

template<RectangleType rectangleType>
float RectangleInsideGpu<rectangleType>::percentToPositon(float percent)
{
    return (std::min<float>(percent, 100)/50) -2;
}

template<RectangleType rectangleType>
const char* RectangleInsideGpu<rectangleType>::getVertexShader()
{
    const char* shaderUsedWithColorsProvidedByUser = R"(#version 330 core
    #extension GL_ARB_explicit_uniform_location : require
    layout (location = 0) in vec2 inPos;
    layout (location = 1) in vec4 inColor;

    layout (location = 0) uniform float posOffset;
    out vec4 vertColor;
    out vec4 calculatedPosition;

    void main()
    {
        gl_Position = vec4(inPos+vec2(0,posOffset), 0.f, 1.f);
        calculatedPosition = vec4(inPos+vec2(0,posOffset), 0.f, 1.f);
        vertColor = inColor;
    })";

    return shaderUsedWithColorsProvidedByUser;
}

template<RectangleType rectangleType>
void RectangleInsideGpu<rectangleType>::updateTime(const float timeInMilliSeconds)
{
    glBindProgramPipeline(pipeline);
    glProgramUniform1f(fs, timeLoc, timeInMilliSeconds);
}

template<RectangleType rectangleType>
void RectangleInsideGpu<rectangleType>::updateBoundary(const float xBegin, const float xEnd)
{
    glBindProgramPipeline(pipeline);
    glProgramUniform2f(fs, boundaryLoc, xBegin, xEnd);
}

template<RectangleType rectangleType>
void RectangleInsideGpu<rectangleType>::updateThemeNumber(const uint16_t themeNumber)
{
    glBindProgramPipeline(pipeline);
    glProgramUniform1ui(fs, themeNumberLoc, themeNumber);
}

template<RectangleType rectangleType>
void RectangleInsideGpu<rectangleType>::move(const float y)
{
    glBindProgramPipeline(pipeline);
    glBindVertexArray(vao);
    glProgramUniform1f(vs, 0, percentToPositon(y));
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

template<RectangleType rectangleType>
void RectangleInsideGpu<rectangleType>::draw()
{
    move(0);
}

template class RectangleInsideGpu<RectangleType::BAR>;
template class RectangleInsideGpu<RectangleType::TRANSPARENT_BAR>;
template class RectangleInsideGpu<RectangleType::BACKGROUND>;

