/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "OpenGlMock.hpp"

PFNGLBINDPROGRAMPIPELINEPROC glad_glBindProgramPipeline = nullptr;
PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = nullptr;
PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation = nullptr;
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
PFNGLPROGRAMUNIFORM1UIPROC glad_glProgramUniform1ui = nullptr;
PFNGLPROGRAMUNIFORM1FPROC glad_glProgramUniform1f = nullptr;
PFNGLPROGRAMUNIFORM2FPROC glad_glProgramUniform2f = nullptr;
PFNGLPROGRAMUNIFORM4FPROC glad_glProgramUniform4f = nullptr;
PFNGLUSEPROGRAMSTAGESPROC glad_glUseProgramStages = nullptr;
PFNGLVERTEXARRAYATTRIBBINDINGPROC glad_glVertexArrayAttribBinding = nullptr;
PFNGLVERTEXARRAYATTRIBFORMATPROC glad_glVertexArrayAttribFormat = nullptr;
PFNGLVERTEXARRAYVERTEXBUFFERPROC glad_glVertexArrayVertexBuffer = nullptr;
PFNGLVIEWPORTPROC glad_glViewport = nullptr;
PFNGLENABLEPROC glad_glEnable = nullptr;
PFNGLBLENDFUNCPROC glad_glBlendFunc = nullptr;

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
std::function<void(GLsizei, GLuint *)> glGenVertexArraysFunction;
std::function<GLint(GLuint, const GLchar *)> glGetUniformLocationFunction;
std::function<void(GLuint, GLint, GLuint)> glProgramUniform1uiFunction;
std::function<void(GLuint, GLint, GLfloat)> glProgramUniform1fFunction;
std::function<void(GLuint, GLint, GLfloat, GLfloat)> glProgramUniform2fFunction;
std::function<void(GLuint, GLint, GLfloat, GLfloat, GLfloat, GLfloat)> glProgramUniform4fFunction;
std::function<void(GLenum, GLint, GLsizei)> glDrawArraysFunction;
std::function<void(GLsizei, const GLuint *)> glDeleteProgramPipelinesFunction;
std::function<void(GLuint)> glDeleteProgramFunction;
std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> glClearColorFunction;
std::function<void(GLbitfield)> glClearFunction;
std::function<void(GLint, GLint,GLsizei, GLsizei)> glViewportFunction;
std::function<void(GLenum)> glEnableFunction;
std::function<void(GLenum, GLenum)> glBlendFuncFunction;

int gladLoadGL(void)
{
    return gladLoadGLFunction();
}

int gladLoadGLLoader(GLADloadproc load)
{
    return 0;
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

void glGenVertexArraysMock(GLsizei n, GLuint *arrays)
{
    glGenVertexArraysFunction(n, arrays);
}

GLint glGetUniformLocationMock(GLuint program, const GLchar *name)
{
    return glGetUniformLocationFunction(program, name);
}
void glProgramUniform1uiMock(GLuint program, GLint location, GLuint v0)
{
    glProgramUniform1fFunction(program, location, v0);
}

void glProgramUniform1fMock(GLuint program, GLint location, GLfloat v0)
{
    glProgramUniform1fFunction(program, location, v0);
}

void glProgramUniform2fMock(GLuint program, GLint location, GLfloat v0, GLfloat v1)
{
    glProgramUniform2fFunction(program, location, v0, v1);
}

void glProgramUniform4fMock(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    glProgramUniform4fFunction(program, location, v0, v1, v2, v3);
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

void glViewportMock(GLint x, GLint y, GLsizei width, GLsizei height)
{
    glViewportFunction(x,y,width,height);
}

void glEnableMock(GLenum cap)
{
    glEnableFunction(cap);
}

void glBlendFuncMock(GLenum sfactor, GLenum dfactor)
{
    glBlendFuncFunction(sfactor,dfactor);
}


OpenGlMock::OpenGlMock()
{

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
    ::glad_glGenVertexArrays = glGenVertexArraysMock;
    ::glad_glGetUniformLocation = glGetUniformLocationMock;
    ::glad_glProgramUniform1ui = glProgramUniform1uiMock;
    ::glad_glProgramUniform1f = glProgramUniform1fMock;
    ::glad_glProgramUniform2f = glProgramUniform2fMock;
    ::glad_glProgramUniform4f = glProgramUniform4fMock;
    ::glad_glDrawArrays = glDrawArraysMock;
    ::glad_glDeleteProgramPipelines = glDeleteProgramPipelinesMock;
    ::glad_glDeleteProgram = glDeleteProgramMock;
    ::glad_glClearColor = glClearColorMock;
    ::glad_glClear = glClearMock;
    ::glad_glViewport = glViewportMock;
    ::glad_glEnable = glEnableMock;
    ::glad_glBlendFunc = glBlendFuncMock;

    gladLoadGLFunction = [this]()
    {
        return this->gladLoadGL();
    };

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

    glGenVertexArraysFunction = [this](GLsizei n, GLuint *arrays)
    {
        this->glGenVertexArrays(n,arrays);
    };

    glGetUniformLocationFunction = [this](GLuint program, const GLchar *name)
    {
        return this->glGetUniformLocation(program, name);
    };

    glProgramUniform1uiFunction = [this](GLuint program, GLint location, GLuint v0)
    {
        this->glProgramUniform1ui(program, location, v0);
    };

    glProgramUniform1fFunction = [this](GLuint program, GLint location, GLfloat v0)
    {
        this->glProgramUniform1f(program, location, v0);
    };

    glProgramUniform2fFunction = [this](GLuint program, GLint location, GLfloat v0, GLfloat v1)
    {
        this->glProgramUniform2f(program, location, v0, v1);
    };

    glProgramUniform4fFunction = [this](GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        this->glProgramUniform4f(program, location, v0, v1, v2, v3);
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

    glViewportFunction = [this](GLint x, GLint y, GLsizei width, GLsizei height)
    {
        this->glViewport(x,y,width,height);
    };

    glEnableFunction = [this](GLenum cap)
    {
        this->glEnable(cap);
    };

    glBlendFuncFunction =  [this](GLenum sfactor, GLenum dfactor)
    {
        this->glBlendFunc(sfactor, dfactor);
    };

}

