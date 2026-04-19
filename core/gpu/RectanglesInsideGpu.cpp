/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "RectanglesInsideGpu.hpp"

template<RectangleType rectangleType>
GLuint RectanglesInsideGpu<rectangleType>::vs = 0;

template<RectangleType rectangleType>
GLuint RectanglesInsideGpu<rectangleType>::fs = 0;

template<RectangleType rectangleType>
GLuint RectanglesInsideGpu<rectangleType>::pipeline = 0;

template<RectangleType rectangleType>
GLuint RectanglesInsideGpu<rectangleType>::timeLoc = 0;

template<RectangleType rectangleType>
GLuint RectanglesInsideGpu<rectangleType>::boundaryLoc = 0;


template<RectangleType rectangleType>
RectanglesInsideGpu<rectangleType>::RectanglesInsideGpu(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle):
    instances(std::vector<Instance>(rectangles.size()))
{

    const auto &rectangle = rectangles.at(0);

    for (uint32_t i = 0; i < instances.size(); i++)
    {
        instances[i].x = rectangles.at(i).at(0).x - rectangle.at(0).x;
        instances[i].y = 0.0f;
    }

    const std::vector<Vertex> rect =
        {
            {rectangle.at(0).x, rectangle.at(0).y, colorsOfRectangle.at(0).at(indexOfRed), colorsOfRectangle.at(0).at(indexOfGreen),colorsOfRectangle.at(0).at(indexOfBlue), colorsOfRectangle.at(0).at(indexOfTransparency)},  //0
            {rectangle.at(1).x, rectangle.at(1).y, colorsOfRectangle.at(1).at(indexOfRed), colorsOfRectangle.at(1).at(indexOfGreen),colorsOfRectangle.at(1).at(indexOfBlue), colorsOfRectangle.at(1).at(indexOfTransparency)},  //1
            {rectangle.at(2).x, rectangle.at(2).y, colorsOfRectangle.at(2).at(indexOfRed), colorsOfRectangle.at(2).at(indexOfGreen),colorsOfRectangle.at(2).at(indexOfBlue), colorsOfRectangle.at(2).at(indexOfTransparency)},  //2
            {rectangle.at(3).x, rectangle.at(3).y, colorsOfRectangle.at(3).at(indexOfRed), colorsOfRectangle.at(3).at(indexOfGreen),colorsOfRectangle.at(3).at(indexOfBlue), colorsOfRectangle.at(3).at(indexOfTransparency)}   //3
        };


    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vertexBuffer);

    glNamedBufferStorage(vertexBuffer,rect.size() * sizeof(Vertex),rect.data(),0);

    glVertexArrayVertexBuffer(vao, BINDING_VERTEX, vertexBuffer, 0, sizeof(Vertex));

    glEnableVertexArrayAttrib(vao, ATTR_POS);
    glVertexArrayAttribFormat(vao, ATTR_POS, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, ATTR_POS, BINDING_VERTEX);

    glEnableVertexArrayAttrib(vao, ATTR_COLOR);
    glVertexArrayAttribFormat(vao, ATTR_COLOR, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, r));
    glVertexArrayAttribBinding(vao, ATTR_COLOR, BINDING_VERTEX);

    glCreateBuffers(1, &instancesBuffer);

    glNamedBufferStorage(instancesBuffer,instances.size() * sizeof(Instance),nullptr,GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(instancesBuffer,0,instances.size() * sizeof(Instance),instances.data());

    glVertexArrayVertexBuffer(vao, BINDING_INSTANCE, instancesBuffer, 0, sizeof(Instance));

    glEnableVertexArrayAttrib(vao, ATTR_OFFSET);
    glVertexArrayAttribFormat(vao, ATTR_OFFSET, 2, GL_FLOAT, GL_FALSE, offsetof(Instance, x));
    glVertexArrayAttribBinding(vao, ATTR_OFFSET, BINDING_INSTANCE);

    glVertexArrayBindingDivisor(vao, BINDING_INSTANCE, 1);
}

template<RectangleType rectangleType>
void RectanglesInsideGpu<rectangleType>::initialize(const char *fsConfig)
{
    prepareShaders(pipeline, vs, fs, getVertexShader(),fsConfig);
    timeLoc = glGetUniformLocation(fs, "timeInMilliSeconds");
    boundaryLoc = glGetUniformLocation(fs, "boundary");
}

template<RectangleType rectangleType>
void RectanglesInsideGpu<rectangleType>::finalize()
{
    ElementInsideGpu::removeShaders(pipeline, vs,fs);
}

template<RectangleType rectangleType>
float RectanglesInsideGpu<rectangleType>::percentToPositon(float percent)
{
    return (std::min<float>(percent, 100)/50) -2;
}

template<RectangleType rectangleType>
const char* RectanglesInsideGpu<rectangleType>::getVertexShader()
{
    const char *shaderUsedWithColorsProvidedByUser = R"(
#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 offset;

out vec4 vColor;
out vec4 calculatedPosition;

void main()
{
    gl_Position = vec4(pos + offset, 0.0, 1.0);
    calculatedPosition = gl_Position;
    vColor = vertexColor;
}
)";

    return shaderUsedWithColorsProvidedByUser;
}

template<RectangleType rectangleType>
void RectanglesInsideGpu<rectangleType>::updateTime(const float timeInMilliSeconds)
{
    glBindProgramPipeline(pipeline);
    glProgramUniform1f(fs, timeLoc, timeInMilliSeconds);
}

template<RectangleType rectangleType>
void RectanglesInsideGpu<rectangleType>::updateBoundary(const float xBegin, const float xEnd)
{
    glBindProgramPipeline(pipeline);
    glProgramUniform2f(fs, boundaryLoc, xBegin, xEnd);
}

template<RectangleType rectangleType>
void RectanglesInsideGpu<rectangleType>::move(const std::vector<float> &positionsInPercents)
{
    for(uint32_t i=0;i<std::min(instances.size(), positionsInPercents.size());++i)
    {
        instances[i].y = percentToPositon(positionsInPercents.at(i));
    }

    glNamedBufferSubData(instancesBuffer,0,instances.size() * sizeof(Instance), instances.data());

    glBindProgramPipeline(pipeline);
    glBindVertexArray(vao);

    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instances.size());
}

template<RectangleType rectangleType>
void RectanglesInsideGpu<rectangleType>::draw()
{
    move(std::vector<float>(instances.size(),0));
}


template class RectanglesInsideGpu<RectangleType::BAR>;
template class RectanglesInsideGpu<RectangleType::SECONDARY_BAR>;
template class RectanglesInsideGpu<RectangleType::BACKGROUND>;

