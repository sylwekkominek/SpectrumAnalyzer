/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "OpenGlMock.hpp"

PFNGLBINDPROGRAMPIPELINEPROC glad_glBindProgramPipeline = nullptr;
PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = nullptr;
PFNGLCLEARPROC glad_glClear = nullptr;
PFNGLCLEARCOLORPROC glad_glClearColor = nullptr;
PFNGLCREATEBUFFERSPROC glad_glCreateBuffers = nullptr;
PFNGLCREATEPROGRAMPIPELINESPROC glad_glCreateProgramPipelines = nullptr;
PFNGLCREATESHADERPROGRAMVPROC glad_glCreateShaderProgramv = nullptr;
PFNGLCREATEVERTEXARRAYSPROC glad_glCreateVertexArrays = nullptr;
PFNGLDELETEPROGRAMPROC glad_glDeleteProgram = nullptr;
PFNGLDELETEPROGRAMPIPELINESPROC glad_glDeleteProgramPipelines = nullptr;
PFNGLDRAWARRAYSPROC glad_glDrawArrays = nullptr;
PFNGLENABLEVERTEXARRAYATTRIBPROC glad_glEnableVertexArrayAttrib = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog = nullptr;
PFNGLNAMEDBUFFERSTORAGEPROC glad_glNamedBufferStorage = nullptr;
PFNGLPROGRAMUNIFORM2FPROC glad_glProgramUniform2f = nullptr;
PFNGLUSEPROGRAMSTAGESPROC glad_glUseProgramStages = nullptr;
PFNGLVERTEXARRAYATTRIBBINDINGPROC glad_glVertexArrayAttribBinding = nullptr;
PFNGLVERTEXARRAYATTRIBFORMATPROC glad_glVertexArrayAttribFormat = nullptr;
PFNGLVERTEXARRAYVERTEXBUFFERPROC glad_glVertexArrayVertexBuffer = nullptr;



std::function<int()> glfwInitFunction;
std::function<void(int hint, int value)> glfwWindowHintFunction;
std::function<GLFWwindow*(int , int , const char* , GLFWmonitor* , GLFWwindow* )> glfwCreateWindowFunction;
std::function<void(GLFWwindow* window)> glfwMakeContextCurrentFunction;
std::function<void(int interval)> glfwSwapIntervalFunction;
std::function<int()> gladLoadGLFunction;
std::function<void(GLsizei , GLuint *)> glCreateProgramPipelinesFunction;
std::function<GLuint(GLenum, GLsizei, const GLchar *const*)> glCreateShaderProgramvFunction;
std::function<void (GLuint, GLsizei, GLsizei *, GLchar *)> glGetProgramInfoLogFunction;
std::function<void(GLuint, GLbitfield, GLuint)> glUseProgramStagesFunction;
std::function<void(GLsizei, GLuint *)> glCreateVertexArraysFunction;
std::function<void(GLsizei, GLuint *)> glCreateBuffersFunction;
std::function<void(GLuint, GLsizeiptr, const void *, GLbitfield)> glNamedBufferStorageFunction;
std::function<void(GLuint, GLuint)> glEnableVertexArrayAttribFunction;
std::function<void(GLuint, GLuint, GLint, GLenum, GLboolean, GLuint)> glVertexArrayAttribFormatFunction;
std::function<void(GLuint, GLuint, GLuint, GLintptr, GLsizei)> glVertexArrayVertexBufferFunction;
std::function<void(GLuint, GLuint, GLuint)> glVertexArrayAttribBindingFunction;
std::function<void(GLuint)> glBindProgramPipelineFunction;
std::function<void(GLuint)> glBindVertexArrayFunction;
std::function<void(GLuint, GLint, GLfloat, GLfloat)> glProgramUniform2fFunction;
std::function<void(GLenum, GLint, GLsizei)> glDrawArraysFunction;
std::function<void(GLsizei, const GLuint *)> glDeleteProgramPipelinesFunction;
std::function<void(GLuint)> glDeleteProgramFunction;
std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> glClearColorFunction;
std::function<void(GLbitfield)> glClearFunction;
std::function<void(GLFWwindow* window)> glfwSwapBuffersFunction;
std::function<void()> glfwTerminateFunction;
std::function<void()> glfwPollEventsFunction;
std::function<int(GLFWwindow* window, int key)> glfwGetKeyFunction;
std::function<void(GLFWwindow* window, int value)> glfwSetWindowShouldCloseFunction;

int glfwInit()
{
    return glfwInitFunction();
}

void glfwWindowHint(int hint, int value)
{
    glfwWindowHintFunction(hint, value);
}

GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
    return glfwCreateWindowFunction(width, height, title, monitor, share);
}


void glfwMakeContextCurrent(GLFWwindow* window)
{
    glfwMakeContextCurrentFunction(window);
}

void glfwSwapInterval(int interval)
{
    glfwSwapIntervalFunction(interval);
}

int gladLoadGL(void)
{
    return gladLoadGLFunction();
}

int gladLoadGLLoader(GLADloadproc load)
{
    return 0;
}

void glfwSwapBuffers(GLFWwindow* window)
{
    glfwSwapBuffersFunction(window);
}

void glfwTerminate()
{
    glfwTerminateFunction();
}

void glfwPollEvents()
{
    glfwPollEventsFunction();
}

int glfwGetKey(GLFWwindow* window, int key)
{
    return glfwGetKeyFunction(window, key);
}

void glfwSetWindowShouldClose(GLFWwindow* window, int value)
{
    glfwSetWindowShouldCloseFunction(window, value);
}

void glCreateProgramPipelinesMock(GLsizei n, GLuint *pipelines)
{
    glCreateProgramPipelinesFunction(n, pipelines);
}

GLuint glCreateShaderProgramvMock(GLenum type, GLsizei count, const GLchar *const* strings)
{
    return glCreateShaderProgramvFunction(type, count, strings);
}

void glGetProgramInfoLogMock(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    glGetProgramInfoLogFunction(program,bufSize,length,infoLog);
}

void glUseProgramStagesMock(GLuint pipeline, GLbitfield stages, GLuint program)
{
    glUseProgramStagesFunction(pipeline, stages, program);
}

void glCreateVertexArraysMock(GLsizei n, GLuint *arrays)
{
    glCreateVertexArraysFunction(n, arrays);
}

void glCreateBuffersMock(GLsizei n, GLuint *buffers)
{
    glCreateBuffersFunction(n, buffers);
}

void glNamedBufferStorageMock(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags)
{
    glNamedBufferStorageFunction(buffer, size, data, flags);
}

void glEnableVertexArrayAttribMock(GLuint vaobj, GLuint index)
{
    glEnableVertexArrayAttribFunction(vaobj, index);
}

void glVertexArrayAttribFormatMock(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
    glVertexArrayAttribFormatFunction(vaobj, attribindex, size, type, normalized, relativeoffset);
}

void glVertexArrayVertexBufferMock(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
{
    glVertexArrayVertexBufferFunction(vaobj, bindingindex, buffer, offset, stride);
}

void glVertexArrayAttribBindingMock(GLuint vaobj, GLuint attribindex, GLuint bindingindex)
{
    glVertexArrayAttribBindingFunction(vaobj, attribindex, bindingindex);
}

void glBindProgramPipelineMock(GLuint pipeline)
{
    glBindProgramPipelineFunction(pipeline);
}
void glBindVertexArrayMock(GLuint array)
{
    glBindVertexArrayFunction(array);
}
void glProgramUniform2fMock(GLuint program, GLint location, GLfloat v0, GLfloat v1)
{
    glProgramUniform2fFunction(program, location, v0, v1);
}
void glDrawArraysMock(GLenum mode, GLint first, GLsizei count)
{
    glDrawArraysFunction(mode, first, count);
}
void glDeleteProgramPipelinesMock(GLsizei n, const GLuint *pipelines)
{
    glDeleteProgramPipelinesFunction(n, pipelines);
}
void glDeleteProgramMock(GLuint program)
{
    glDeleteProgramFunction(program);
}

void glClearColorMock(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glClearColorFunction(red, green, blue, alpha);
}

void glClearMock(GLbitfield mask)
{
    glClearFunction(mask);
}

OpenGlMock::OpenGlMock()
{

    glfwInitFunction = [this]()
    {
        return this->glfwInit();
    };

    glfwWindowHintFunction = [this](int hint, int value)
    {
        return this->glfwWindowHint(hint,value);
    };


    glfwCreateWindowFunction = [this](int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
    {
        return this->glfwCreateWindow(width, height, title, monitor, share);
    };


    glfwMakeContextCurrentFunction = [this](GLFWwindow* window)
    {
        return this->glfwMakeContextCurrent(window);
    };

    glfwSwapIntervalFunction = [this](int interval)
    {
        return this->glfwSwapInterval(interval);
    };

    gladLoadGLFunction = [this]()
    {
        return this->gladLoadGL();
    };

    glfwSwapBuffersFunction = [this](GLFWwindow* window)
    {
        this->glfwSwapBuffers(window);
    };

    glfwTerminateFunction = [this]()
    {
        this->glfwTerminate();
    };

    glfwPollEventsFunction = [this]()
    {
        this->glfwPollEvents();
    };

    glfwGetKeyFunction = [this](GLFWwindow* window, int key)
    {
        return this->glfwGetKey(window, key);
    };

    glfwSetWindowShouldCloseFunction = [this](GLFWwindow* window, int value)
    {
        this->glfwSetWindowShouldClose(window, value);
    };


    ::glad_glCreateProgramPipelines = glCreateProgramPipelinesMock;
    ::glad_glCreateShaderProgramv = glCreateShaderProgramvMock;
    ::glad_glGetProgramInfoLog = glGetProgramInfoLogMock;
    ::glad_glUseProgramStages = glUseProgramStagesMock;
    ::glad_glCreateVertexArrays = glCreateVertexArraysMock;
    ::glad_glCreateBuffers = glCreateBuffersMock;
    ::glad_glNamedBufferStorage = glNamedBufferStorageMock;
    ::glad_glEnableVertexArrayAttrib = glEnableVertexArrayAttribMock;
    ::glad_glVertexArrayAttribFormat = glVertexArrayAttribFormatMock;
    ::glad_glVertexArrayVertexBuffer = glVertexArrayVertexBufferMock;
    ::glad_glVertexArrayAttribBinding = glVertexArrayAttribBindingMock;
    ::glad_glBindProgramPipeline = glBindProgramPipelineMock;
    ::glad_glBindVertexArray = glBindVertexArrayMock;
    ::glad_glProgramUniform2f = glProgramUniform2fMock;
    ::glad_glDrawArrays = glDrawArraysMock;
    ::glad_glDeleteProgramPipelines = glDeleteProgramPipelinesMock;
    ::glad_glDeleteProgram = glDeleteProgramMock;
    ::glad_glClearColor = glClearColorMock;
    ::glad_glClear = glClearMock;

    glCreateShaderProgramvFunction = [this](GLenum type, GLsizei count, const GLchar *const* strings)
    {
        return this->glCreateShaderProgramv(type, count, strings);
    };

    glGetProgramInfoLogFunction = [this](GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
    {
        this->glGetProgramInfoLog(program,bufSize,length,infoLog);
    };

    glCreateProgramPipelinesFunction = [this](GLsizei n, GLuint *pipelines)
    {
        this->glCreateProgramPipelines(n, pipelines);
    };

    glUseProgramStagesFunction = [this](GLuint pipeline, GLbitfield stages, GLuint program)
    {
        this->glUseProgramStages(pipeline, stages, program);
    };

    glCreateVertexArraysFunction =[this](GLsizei n, GLuint *arrays)
    {
        this->glCreateVertexArrays(n, arrays);
    };

    glCreateBuffersFunction = [this](GLsizei n, GLuint *buffers)
    {
        this->glCreateBuffers(n, buffers);
    };

    glNamedBufferStorageFunction= [this](GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags)
    {
        this->glNamedBufferStorage(buffer, size, data, flags);
    };

    glEnableVertexArrayAttribFunction= [this](GLuint vaobj, GLuint index)
    {
        this->glEnableVertexArrayAttrib(vaobj, index);
    };

    glVertexArrayAttribFormatFunction= [this](GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
    {
        this->glVertexArrayAttribFormat(vaobj, attribindex, size, type, normalized, relativeoffset);
    };

    glVertexArrayVertexBufferFunction= [this](GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
    {
        this->glVertexArrayVertexBuffer(vaobj, bindingindex, buffer, offset, stride);
    };

    glVertexArrayAttribBindingFunction= [this](GLuint vaobj, GLuint attribindex, GLuint bindingindex)
    {
        this->glVertexArrayAttribBinding(vaobj, attribindex, bindingindex);
    };

    glBindProgramPipelineFunction = [this](GLuint pipeline)
    {
        this->glBindProgramPipeline(pipeline);
    };

    glBindVertexArrayFunction = [this](GLuint array)
    {
        this->glBindVertexArray(array);
    };

    glProgramUniform2fFunction = [this](GLuint program, GLint location, GLfloat v0, GLfloat v1)
    {
        this->glProgramUniform2f(program, location, v0, v1);
    };

    glDrawArraysFunction = [this](GLenum mode, GLint first, GLsizei count)
    {
        this->glDrawArrays(mode, first, count);
    };

    glDeleteProgramPipelinesFunction = [this](GLsizei n, const GLuint *pipelines)
    {
        this->glDeleteProgramPipelines(n, pipelines);
    };

    glDeleteProgramFunction = [this](GLuint program)
    {
        this->glDeleteProgram(program);
    };

    glClearColorFunction = [this](GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        this->glClearColor(red, green, blue, alpha);
    };

    glClearFunction = [this](GLbitfield mask)
    {
        this->glClear(mask);
    };

}

