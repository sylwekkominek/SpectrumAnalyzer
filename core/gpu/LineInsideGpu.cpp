/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "LineInsideGpu.hpp"

GLuint LineInsideGpu::vs = 0;
GLuint LineInsideGpu::fs = 0;
GLuint LineInsideGpu::pipeline = 0;
GLint LineInsideGpu::p0Loc = 0;
GLint LineInsideGpu::p1Loc = 0;
GLint LineInsideGpu::colorLoc = 0;


LineInsideGpu::LineInsideGpu()
{
    glGenVertexArrays(1, &vao);
}

void LineInsideGpu::draw(const Line &line, const std::vector<float> &color)
{
    glBindProgramPipeline(pipeline);
    glBindVertexArray(vao);
    glProgramUniform2f(vs, p0Loc, percentToPositon(line.at(0).x), percentToPositon(line.at(0).y));
    glProgramUniform2f(vs, p1Loc, percentToPositon(line.at(1).x), percentToPositon(line.at(1).y));
    glProgramUniform4f(fs, colorLoc, color.at(indexOfRed), color.at(indexOfGreen), color.at(indexOfBlue), color.at(indexOfTransparency));

    glDrawArrays(GL_LINES, 0, 2);
}

float LineInsideGpu::percentToPositon(float percent)
{
    return (std::min<float>(percent, 100)/50) -1.0;
}

void LineInsideGpu::initialize()
{
    prepareShaders(pipeline, vs, fs, getVertexShader(), getFragmentShader());
    p0Loc = glGetUniformLocation(vs, "p0");
    p1Loc = glGetUniformLocation(vs, "p1");
}

void LineInsideGpu::finalize()
{
    ElementInsideGpu::removeShaders(pipeline, vs,fs);
}

const char* LineInsideGpu::getVertexShader()
{
    const char* shaderUsedWithColorsProvidedByUser = R"(#version 330 core
uniform vec2 p0;
uniform vec2 p1;

void main() {
    vec2 pos = (gl_VertexID == 0) ? p0 : p1;
    gl_Position = vec4(pos, 0.0, 1.0);
})";

    return shaderUsedWithColorsProvidedByUser;

}

const char* LineInsideGpu::getFragmentShader()
{
    const char* fragmentShaderSrc = R"(
#version 330 core
uniform vec4 uColor;
out vec4 Color;

void main() {
    Color = uColor;
}
)";

    return fragmentShaderSrc;
}
