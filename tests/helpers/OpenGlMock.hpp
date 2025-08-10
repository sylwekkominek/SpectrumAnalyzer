/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gmock/gmock.h>


class OpenGlMock
{
public:
    OpenGlMock();

    MOCK_METHOD0(glfwInit, int());
    MOCK_METHOD2(glfwWindowHint, void(int hint, int value));
    MOCK_METHOD5(glfwCreateWindow, GLFWwindow*(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share));
    MOCK_METHOD1(glfwMakeContextCurrent, void(GLFWwindow* window));
    MOCK_METHOD1(glfwSwapInterval, void(int interval));
    MOCK_METHOD0(gladLoadGL, int());
    MOCK_METHOD1(glfwSwapBuffers, void(GLFWwindow* window));
    MOCK_METHOD0(glfwTerminate, void());
    MOCK_METHOD0(glfwPollEvents, void());
    MOCK_METHOD2(glfwGetKey, int(GLFWwindow* window, int key));
    MOCK_METHOD2(glfwGetWindowAttrib, int(GLFWwindow* window, int attrib));
    MOCK_METHOD1(glfwWindowShouldClose, int(GLFWwindow* window));
    MOCK_METHOD2(glfwSetWindowShouldClose, void(GLFWwindow* window, int value));
    MOCK_METHOD2(glfwSetFramebufferSizeCallback, GLFWframebuffersizefun(GLFWwindow* window, GLFWframebuffersizefun callback));
    MOCK_METHOD2(glfwSetWindowMaximizeCallback, GLFWwindowmaximizefun(GLFWwindow* window, GLFWwindowmaximizefun callback));
    MOCK_METHOD1(glfwDestroyWindow, void(GLFWwindow* glfwWindow));

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



