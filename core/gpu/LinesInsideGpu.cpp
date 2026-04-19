/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "LinesInsideGpu.hpp"

GLuint LinesInsideGpu::vs = 0;
GLuint LinesInsideGpu::fs = 0;
GLuint LinesInsideGpu::pipeline = 0;


LinesInsideGpu::LinesInsideGpu(const uint32_t numberOfLines, const std::vector<float> &color) : instances(numberOfLines)
{
    const std::vector<Vertex> colors =
    {
        {color.at(0),color.at(1),color.at(2),color.at(3)},
        {color.at(0),color.at(1),color.at(2),color.at(3)}
    };

    glCreateVertexArrays(1, &vao);

    glCreateBuffers(1, &colorBuffer);
    glNamedBufferStorage(colorBuffer,colors.size() * sizeof(Vertex),colors.data(),0);

    glEnableVertexArrayAttrib(vao, ATTR_COLOR);
    glVertexArrayAttribFormat(vao, ATTR_COLOR, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(vao, BINDING_COLOR, colorBuffer, 0, sizeof(Vertex));
    glVertexArrayAttribBinding(vao, ATTR_COLOR, BINDING_COLOR);

    glCreateBuffers(1, &instanceBuffer);
    glNamedBufferStorage(instanceBuffer,instances.size() * sizeof(Instance),nullptr,GL_DYNAMIC_STORAGE_BIT);

    glEnableVertexArrayAttrib(vao, ATTR_P0);
    glVertexArrayAttribFormat(vao, ATTR_P0, 2, GL_FLOAT, GL_FALSE, offsetof(Instance, x0));
    glVertexArrayAttribBinding(vao, ATTR_P0, BINDING_INSTANCE);

    glEnableVertexArrayAttrib(vao, ATTR_P1);
    glVertexArrayAttribFormat(vao, ATTR_P1, 2, GL_FLOAT, GL_FALSE, offsetof(Instance, x1));
    glVertexArrayAttribBinding(vao, ATTR_P1, BINDING_INSTANCE);

    glVertexArrayVertexBuffer(vao, BINDING_INSTANCE, instanceBuffer, 0, sizeof(Instance));
    glVertexArrayBindingDivisor(vao, BINDING_INSTANCE, 1);
}

void LinesInsideGpu::draw(const Lines &lines)
{
    for (int i = 0; i < std::min(instances.size(), lines.size()); i++)
    {
        instances[i].x0 = percentToPositon(lines[i][0].x);
        instances[i].y0 = percentToPositon(lines[i][0].y);

        instances[i].x1 = percentToPositon(lines[i][1].x);
        instances[i].y1 = percentToPositon(lines[i][1].y);
    }

    glNamedBufferSubData(instanceBuffer,
                         0,
                         instances.size() * sizeof(Instance),
                         instances.data());


    glBindProgramPipeline(pipeline);
    glBindVertexArray(vao);

    glDrawArraysInstanced(GL_LINES, 0, 2, instances.size());
}

float LinesInsideGpu::percentToPositon(float percent)
{
    return (std::min<float>(percent, 100)/50) -1.0;
}

void LinesInsideGpu::initialize()
{
    prepareShaders(pipeline, vs, fs, getVertexShader(), getFragmentShader());
}

void LinesInsideGpu::finalize()
{
    ElementInsideGpu::removeShaders(pipeline, vs,fs);
}

const char* LinesInsideGpu::getVertexShader()
{
    const char* shaderUsedWithColorsProvidedByUser = R"(
#version 330 core

layout(location = 0) in vec4 color;   // VBO (binding 0)
layout(location = 1) in vec2 p0;      // instance
layout(location = 2) in vec2 p1;      // instance

out vec4 vColor;

void main()
{
    vec2 pos = (gl_VertexID == 0) ? p0 : p1;

    gl_Position = vec4(pos, 0.0, 1.0);
    vColor = color;
}

)";
    return shaderUsedWithColorsProvidedByUser;
}

const char* LinesInsideGpu::getFragmentShader()
{
    const char* fragmentShaderSrc = R"(
#version 330 core

in vec4 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vColor;
}
)";
    return fragmentShaderSrc;
}
