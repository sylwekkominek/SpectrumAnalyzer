/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "OpenGlMock.hpp"
#include <gtest/gtest.h>

using::testing::_;
using::testing::Return;
using ::testing::InSequence;

static const double marginOfError{exp(-3)};

MATCHER_P(isInRange, value, "")
{
    if(std::abs(arg-value)>marginOfError)
    {
        return false;
    }

    return true;
}


struct WindowTestsBase
{
    OpenGlMock openGL;


    void expectCreateWindow()
    {
        EXPECT_CALL(openGL, glfwInit()).Times(1);
        EXPECT_CALL(openGL, glfwWindowHint(_,_)).Times(4);
        EXPECT_CALL(openGL, glfwCreateWindow(_,_,_,_,_)).Times(1);
        EXPECT_CALL(openGL, glfwSetFramebufferSizeCallback(_,_)).Times(1);

        EXPECT_CALL(openGL, glfwMakeContextCurrent(_)).Times(1);
        EXPECT_CALL(openGL, glfwSwapInterval(_)).Times(1);
        EXPECT_CALL(openGL, gladLoadGL()).Times(1);
    }

    void expectInitializeGPU(const uint numberOfRectangles, const bool smallRectanglesEnabled)
    {
        const uint numberOfExpectCalls = smallRectanglesEnabled ? 2 * numberOfRectangles : numberOfRectangles;

        EXPECT_CALL(openGL, glCreateShaderProgramv(_,_,_)).Times(4);
        EXPECT_CALL(openGL, glGetProgramInfoLog(_,_,_,_)).Times(4);
        EXPECT_CALL(openGL, glCreateProgramPipelines(_,_)).Times(2);
        EXPECT_CALL(openGL, glUseProgramStages(_,_,_)).Times(4);
        EXPECT_CALL(openGL, glGetUniformLocation(_,_)).Times(3);


        EXPECT_CALL(openGL, glCreateVertexArrays(_,_)).Times(numberOfExpectCalls);
        EXPECT_CALL(openGL, glCreateBuffers(_,_)).Times(2*numberOfExpectCalls);
        EXPECT_CALL(openGL, glNamedBufferStorage(_,_,_,_)).Times(2*numberOfExpectCalls);

        EXPECT_CALL(openGL, glEnableVertexArrayAttrib(_,_)).Times(2*numberOfExpectCalls);
        EXPECT_CALL(openGL, glVertexArrayAttribFormat(_,_,_,_,_,_)).Times(2*numberOfExpectCalls);
        EXPECT_CALL(openGL, glVertexArrayVertexBuffer(_,_,_,_,_)).Times(2*numberOfExpectCalls);
        EXPECT_CALL(openGL, glVertexArrayAttribBinding(_,_,_)).Times(2*numberOfExpectCalls);
    }

    void expectDraw(const uint numberOfRectangles, const bool smallRectanglesEnabled)
    {
        const uint numberOfExpectCalls = smallRectanglesEnabled ? 2 * numberOfRectangles : numberOfRectangles;
        const uint timeUpdateCall = 1;
        EXPECT_CALL(openGL, glClear(_)).Times(1);
        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(numberOfExpectCalls+timeUpdateCall);
        EXPECT_CALL(openGL, glBindVertexArray(_)).Times(numberOfExpectCalls);
        EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(numberOfExpectCalls+timeUpdateCall);
        EXPECT_CALL(openGL, glDrawArrays(_,_,_)).Times(numberOfExpectCalls);
        EXPECT_CALL(openGL, glfwSwapBuffers(_)).Times(1);
    }

    void expectDraw(const std::vector<float> &positions, const bool smallRectanglesEnabled)
    {
        const uint numberOfRectangles = positions.size();
        const uint numberOfSmallRectangles = positions.size();

        InSequence s;

        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(1);
        EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(1);

        EXPECT_CALL(openGL, glClear(_)).Times(1);

        for(auto &el: positions)
        {
            EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(1);
            EXPECT_CALL(openGL, glBindVertexArray(_)).Times(1);
            EXPECT_CALL(openGL, glProgramUniform1f(_,_,isInRange(el))).Times(1);
            EXPECT_CALL(openGL, glDrawArrays(_,_,_)).Times(1);
        }

        if(smallRectanglesEnabled)
        {
            for(auto &el: positions)
            {
                EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(1);
                EXPECT_CALL(openGL, glBindVertexArray(_)).Times(1);
                EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(1);
                EXPECT_CALL(openGL, glDrawArrays(_,_,_)).Times(1);
            }
        }

        EXPECT_CALL(openGL, glfwSwapBuffers(_)).Times(1);
    }

    void expectCheckIfWindowShouldBeClosed(const bool returnValue=true)
    {
        EXPECT_CALL(openGL, glfwPollEvents()).Times(1).RetiresOnSaturation();
        EXPECT_CALL(openGL, glfwWindowShouldClose(_)).WillOnce(Return(returnValue));
    }

    void expectCheckIfWindowShouldRecreated()
    {
        EXPECT_CALL(openGL, glfwPollEvents()).Times(1).RetiresOnSaturation();
        EXPECT_CALL(openGL, glfwGetWindowAttrib(_,_)).WillOnce(Return(false));
    }

    void expectDestroyWindow()
    {
        EXPECT_CALL(openGL, glDeleteProgramPipelines(_,_)).Times(2);
        EXPECT_CALL(openGL, glDeleteProgram(_)).Times(4);
        EXPECT_CALL(openGL, glfwDestroyWindow(_)).Times(1);
    }

};
