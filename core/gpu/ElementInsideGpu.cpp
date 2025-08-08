/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "ElementInsideGpu.hpp"
#include <iostream>


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
