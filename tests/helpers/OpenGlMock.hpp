/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <glad/glad.h>
#include <gmock/gmock.h>


class OpenGlMock
{
public:
    OpenGlMock();

    MOCK_METHOD0(gladLoadGL, int());
    MOCK_METHOD2(glCreateVertexArrays, void(GLsizei, GLuint *));
    MOCK_METHOD2(glCreateBuffers, void(GLsizei, GLuint *));
    MOCK_METHOD4(glNamedBufferStorage, void(GLuint, GLsizeiptr, const void *, GLbitfield));
    MOCK_METHOD2(glEnableVertexArrayAttrib, void(GLuint, GLuint));
    MOCK_METHOD6(glVertexArrayAttribFormat, void(GLuint, GLuint, GLint, GLenum, GLboolean, GLuint));
    MOCK_METHOD5(glVertexArrayVertexBuffer, void(GLuint, GLuint, GLuint, GLintptr, GLsizei));
    MOCK_METHOD3(glVertexArrayAttribBinding, void(GLuint, GLuint, GLuint));
    MOCK_METHOD3(glCreateShaderProgramv, GLuint(GLenum, GLsizei, const GLchar *const*));
    MOCK_METHOD4(glGetProgramInfoLog, void(GLuint, GLsizei, GLsizei *, GLchar *));
    MOCK_METHOD3(glUseProgramStages, void(GLuint pipeline, GLbitfield stages, GLuint program));
    MOCK_METHOD1(glBindProgramPipeline, void(GLuint));
    MOCK_METHOD1(glBindVertexArray, void(GLuint));
    MOCK_METHOD2(glGenVertexArrays, void(GLsizei, GLuint *));
    MOCK_METHOD2(glGetUniformLocation, GLint(GLuint, const GLchar *));
    MOCK_METHOD3(glProgramUniform1f, void(GLuint, GLint, GLfloat));
    MOCK_METHOD4(glProgramUniform2f, void(GLuint, GLint, GLfloat, GLfloat));
    MOCK_METHOD6(glProgramUniform4f, void(GLuint, GLint, GLfloat, GLfloat, GLfloat, GLfloat));
    MOCK_METHOD3(glDrawArrays, void(GLenum, GLint, GLsizei));
    MOCK_METHOD2(glDeleteProgramPipelines, void(GLsizei, const GLuint *));
    MOCK_METHOD1(glDeleteProgram, void(GLuint));
    MOCK_METHOD2(glCreateProgramPipelines, void(GLsizei, GLuint*));
    MOCK_METHOD4(glClearColor, void(GLfloat, GLfloat, GLfloat, GLfloat));
    MOCK_METHOD1(glClear, void(GLbitfield));
    MOCK_METHOD4(glViewport, void(GLint , GLint ,GLsizei , GLsizei));
    MOCK_METHOD1(glEnable, void(GLenum));
    MOCK_METHOD2(glBlendFunc, void(GLenum, GLenum));
};



