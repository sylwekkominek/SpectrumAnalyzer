/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "ElementsInsideGpu.hpp"
#include <iostream>

GLuint RectangleInsideGpu::vs = 0;
GLuint RectangleInsideGpu::fs = 0;
GLuint RectangleInsideGpu::pipeline = 0;


void ElementInsideGpu::prepareShaders(GLuint &pipeline, GLuint &vs, GLuint &fs, const char * vsConfig, const char *fsConfig)
{
    vs = compileShader(vsConfig, GL_VERTEX_SHADER, "VS log");
    fs = compileShader(fsConfig, GL_FRAGMENT_SHADER, "FS log");

    glCreateProgramPipelines(1, &pipeline);
    glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vs);
    glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fs);
}

void ElementInsideGpu::removeShaders(GLuint &pipeline, GLuint &vs, GLuint &fs)
{
    glDeleteProgramPipelines(1, &pipeline);
    glDeleteProgram(vs);
    glDeleteProgram(fs);
}

GLuint ElementInsideGpu::compileShader(const GLchar* source, GLenum stage, const std::string& msg)
{
    GLuint shdr = glCreateShaderProgramv(stage, 1, &source);
    std::string log;
    log.resize(1024);
    glGetProgramInfoLog(shdr, log.size(), nullptr, &log.front());
    std::cout<<msg.c_str()<<": "<<log.c_str()<<std::endl;
    return shdr;
}

RectangleInsideGpu::RectangleInsideGpu(const Rectangle &rectangle)
{
    glCreateVertexArrays(1, &vao);

    glCreateBuffers(1, &vertexBuffer);
    glNamedBufferStorage(vertexBuffer, rectangle.size()*sizeof(Rectangle::value_type), rectangle.data(), 0);

    glEnableVertexArrayAttrib(vao, ATTR_POS);
    glVertexArrayAttribFormat(vao, ATTR_POS, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(vao, ATTR_POS, vertexBuffer, 0, sizeof(Point));
    glVertexArrayAttribBinding(vao, ATTR_POS, ATTR_POS);
}

RectangleInsideGpu::RectangleInsideGpu(const Rectangle &rectangle, const ColorsOfRectanglePerVertices &colorsOfRectangle):
    RectangleInsideGpu(rectangle)
{

    const float colors[]={
        colorsOfRectangle.at(2).at(indexOfRed), colorsOfRectangle.at(2).at(indexOfGreen),colorsOfRectangle.at(2).at(indexOfBlue),colorsOfRectangle.at(2).at(indexOfTransparency),    //2
        colorsOfRectangle.at(1).at(indexOfRed), colorsOfRectangle.at(1).at(indexOfGreen),colorsOfRectangle.at(1).at(indexOfBlue),colorsOfRectangle.at(1).at(indexOfTransparency),    //1
        colorsOfRectangle.at(0).at(indexOfRed), colorsOfRectangle.at(0).at(indexOfGreen),colorsOfRectangle.at(0).at(indexOfBlue),colorsOfRectangle.at(0).at(indexOfTransparency),    //0
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

void RectangleInsideGpu::initialize(const char *fsConfig)
{
    prepareShaders(pipeline, vs, fs, getVertexShader(),fsConfig);
}

void RectangleInsideGpu::finalize()
{
    ElementInsideGpu::removeShaders(pipeline, vs,fs);
}

float RectangleInsideGpu::percentToPositon(float percent)
{
    return (std::min<float>(percent, 100)/50) -2;
}

const char* RectangleInsideGpu::getVertexShader()
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

void RectangleInsideGpu::move(const float y)
{
    glBindProgramPipeline(pipeline);
    glBindVertexArray(vao);
    glProgramUniform1f(vs, 0, percentToPositon(y));
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
