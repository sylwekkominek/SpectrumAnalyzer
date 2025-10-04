/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "TextInsideGpuMock.hpp"
#include "WindowBaseMock.hpp"
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
    TextInsideGpuMock text;
    WindowBaseMock windowBase;
    OpenGlMock openGL;


    void expectCreateWindow()
    {
        EXPECT_CALL(windowBase, createWindow()).Times(1);
    }

    void expectInitializeGPU(const uint32_t numberOfRectangles, const bool smallRectanglesEnabled)
    {
        const uint32_t backgroundCall{1};
        const uint32_t numberOfExpectCalls = (smallRectanglesEnabled ? 2 * numberOfRectangles : numberOfRectangles);

        EXPECT_CALL(openGL, gladLoadGL()).Times(1);
        EXPECT_CALL(openGL, glEnable(GL_BLEND)).Times(1);
        EXPECT_CALL(openGL, glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)).Times(1);

        EXPECT_CALL(openGL, glCreateShaderProgramv(_,_,_)).Times(6);
        EXPECT_CALL(openGL, glGetProgramInfoLog(_,_,_,_)).Times(6);
        EXPECT_CALL(openGL, glCreateProgramPipelines(_,_)).Times(3);
        EXPECT_CALL(openGL, glUseProgramStages(_,_,_)).Times(6);
        EXPECT_CALL(openGL, glGetUniformLocation(_,_)).Times(6);
        EXPECT_CALL(text, initialize()).Times(1);

        EXPECT_CALL(openGL, glCreateVertexArrays(_,_)).Times(numberOfExpectCalls+backgroundCall);
        EXPECT_CALL(openGL, glCreateBuffers(_,_)).Times(2*numberOfExpectCalls+backgroundCall);
        EXPECT_CALL(openGL, glNamedBufferStorage(_,_,_,_)).Times(2*numberOfExpectCalls+backgroundCall);

        EXPECT_CALL(openGL, glEnableVertexArrayAttrib(_,_)).Times(2*numberOfExpectCalls+backgroundCall);
        EXPECT_CALL(openGL, glVertexArrayAttribFormat(_,_,_,_,_,_)).Times(2*numberOfExpectCalls+backgroundCall);
        EXPECT_CALL(openGL, glVertexArrayVertexBuffer(_,_,_,_,_)).Times(2*numberOfExpectCalls+backgroundCall);
        EXPECT_CALL(openGL, glVertexArrayAttribBinding(_,_,_)).Times(2*numberOfExpectCalls+backgroundCall);
    }

    void expectDraw(const uint32_t numberOfRectangles, const bool smallRectanglesEnabled)
    {
        const uint32_t backgroundCall{1};
        const uint32_t numberOfExpectCalls = (smallRectanglesEnabled ? 2 * numberOfRectangles : numberOfRectangles)+backgroundCall;
        const uint32_t timeUpdateCall = 2;
        EXPECT_CALL(openGL, glClear(_)).Times(1);
        EXPECT_CALL(windowBase, getWindowSize).WillOnce(Return(WindowSize{1024,768}));

        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(numberOfExpectCalls+timeUpdateCall);
        EXPECT_CALL(openGL, glBindVertexArray(_)).Times(numberOfExpectCalls);
        EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(numberOfExpectCalls+timeUpdateCall);
        EXPECT_CALL(openGL, glDrawArrays(_,_,_)).Times(numberOfExpectCalls);
        EXPECT_CALL(windowBase, getCursorPosition()).WillOnce(Return(CursorPosition{0,0}));
        EXPECT_CALL(windowBase, swapBuffers()).Times(1);
    }

    void expectDraw(const std::vector<float> &positions, const bool smallRectanglesEnabled)
    {
        const uint32_t backgroundCall{1};
        const uint32_t numberOfRectangles = positions.size();
        const uint32_t numberOfSmallRectangles = positions.size();

        InSequence s;

        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(1);
        EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(1);

        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(1);
        EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(1);

        EXPECT_CALL(openGL, glClear(_)).Times(1);


        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(1);
        EXPECT_CALL(openGL, glBindVertexArray(_)).Times(1);
        EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(1);
        EXPECT_CALL(openGL, glDrawArrays(_,_,_)).Times(1);

        EXPECT_CALL(windowBase, getWindowSize).WillOnce(Return(WindowSize{1024,768}));

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

        for(auto &el: positions)
        {
            EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(1);
            EXPECT_CALL(openGL, glBindVertexArray(_)).Times(1);
            EXPECT_CALL(openGL, glProgramUniform1f(_,_,isInRange(el))).Times(1);
            EXPECT_CALL(openGL, glDrawArrays(_,_,_)).Times(1);
        }

        EXPECT_CALL(windowBase, getCursorPosition()).WillOnce(Return(CursorPosition{0,0}));
        EXPECT_CALL(windowBase, swapBuffers()).Times(1);
    }

    void expectCheckIfWindowShouldBeClosed(const bool returnValue=true)
    {
        EXPECT_CALL(windowBase, checkIfWindowShouldBeClosed()).WillOnce(Return(returnValue));
    }

    void expectCheckIfWindowShouldRecreated()
    {
        EXPECT_CALL(windowBase, checkIfWindowShouldBeRecreated()).WillOnce(Return(false));
    }

    void expectDestroyWindow()
    {
        EXPECT_CALL(openGL, glDeleteProgramPipelines(_,_)).Times(3);
        EXPECT_CALL(openGL, glDeleteProgram(_)).Times(6);
        EXPECT_CALL(text, finalize()).Times(1);
    }

};
