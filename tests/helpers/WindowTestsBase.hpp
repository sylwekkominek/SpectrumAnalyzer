/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "TextInsideGpuMock.hpp"
#include "WindowBaseMock.hpp"
#include "OpenGlMock.hpp"
#include <iostream>
#include <gtest/gtest.h>

using::testing::_;
using::testing::Return;
using ::testing::InSequence;
using ::testing::AnyNumber;

static const double marginOfError{0.001};

MATCHER_P(isInRange, value, "")
{
    if(std::abs(arg-value)>marginOfError)
    {
        std::cout << std::fixed << std::setprecision(8)
                  << "expected: " << value
                  << ", actual: " << arg
                  << ", diff: " << std::abs(value - arg)
                  << ", marginOfError: " << marginOfError
                  << std::endl;


        return false;
    }

    return true;
}


struct WindowTestsBase
{
    TextInsideGpuMock text;
    WindowBaseMock windowBase;
    OpenGlMock openGL;

    const uint32_t backgroundCall{1};
    const uint32_t rectanglesCall{1};
    const uint32_t dynamicMaxholdSecondaryRectanglesCall{1};
    const uint32_t smallRectanglesCall{1};
    const uint32_t linesCall{1};
    const uint32_t dynamicMaxholdLinesCall{1};
    const uint32_t dynamicMaxholdSecondaryLinesCall{1};
    const uint32_t highlightedVerticalLineCall{1};
    const uint32_t horizontalLines{1};
    const uint32_t verticalLines{1};

    struct Instance
    {
        float x, y;
    };

    void expectCreateWindow()
    {
        EXPECT_CALL(windowBase, createWindow()).Times(1);
    }

    void expectInitializeGPU(const uint32_t numberOfRectangles, const bool additionalRectanglesEnabled)
    {
        const uint32_t differentTypesCall{backgroundCall + rectanglesCall + dynamicMaxholdSecondaryRectanglesCall + linesCall};
        const uint32_t numberOfRectanglesCalls = (additionalRectanglesEnabled ? (rectanglesCall + dynamicMaxholdSecondaryRectanglesCall + smallRectanglesCall+backgroundCall) : (rectanglesCall+backgroundCall));
        const uint32_t numberOfLinesCalls = (linesCall + dynamicMaxholdLinesCall + dynamicMaxholdSecondaryLinesCall + highlightedVerticalLineCall + horizontalLines + verticalLines);

        EXPECT_CALL(openGL, gladLoadGL()).Times(1);
        EXPECT_CALL(openGL, glEnable(GL_BLEND)).Times(1);
        EXPECT_CALL(openGL, glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)).Times(1);

        EXPECT_CALL(openGL, glCreateShaderProgramv(_,_,_)).Times(2*differentTypesCall);
        EXPECT_CALL(openGL, glGetProgramInfoLog(_,_,_,_)).Times(2*differentTypesCall);
        EXPECT_CALL(openGL, glCreateProgramPipelines(_,_)).Times(differentTypesCall);
        EXPECT_CALL(openGL, glUseProgramStages(_,_,_)).Times(2*differentTypesCall);
        EXPECT_CALL(openGL, glGetUniformLocation(_,_)).Times(2*(rectanglesCall + dynamicMaxholdSecondaryRectanglesCall + backgroundCall));
        EXPECT_CALL(text, initialize()).Times(1);

        EXPECT_CALL(openGL, glCreateVertexArrays(_,_)).Times(numberOfRectanglesCalls + numberOfLinesCalls);
        EXPECT_CALL(openGL, glCreateBuffers(_,_)).Times(2*(numberOfRectanglesCalls + numberOfLinesCalls));
        EXPECT_CALL(openGL, glNamedBufferStorage(_,_,_,_)).Times(2*(numberOfRectanglesCalls + numberOfLinesCalls));

        EXPECT_CALL(openGL, glEnableVertexArrayAttrib(_,_)).Times(3*(numberOfRectanglesCalls + numberOfLinesCalls));
        EXPECT_CALL(openGL, glVertexArrayAttribFormat(_,_,_,_,_,_)).Times(3*(numberOfRectanglesCalls + numberOfLinesCalls));
        EXPECT_CALL(openGL, glVertexArrayVertexBuffer(_,_,_,_,_)).Times(2*(numberOfRectanglesCalls + numberOfLinesCalls));
        EXPECT_CALL(openGL, glVertexArrayAttribBinding(_,_,_)).Times(3*(numberOfRectanglesCalls + numberOfLinesCalls));

        EXPECT_CALL(openGL, glNamedBufferSubData(_,_,_,_)).Times((numberOfRectanglesCalls));
        EXPECT_CALL(openGL, glVertexArrayBindingDivisor(_,_,_)).Times((numberOfRectanglesCalls + numberOfLinesCalls));
    }

    void expectDraw(const uint32_t numberOfRectangles, const bool additionalRectanglesEnabled)
    {
        const uint32_t numberOfRectanglesCalls = (additionalRectanglesEnabled ? (rectanglesCall + dynamicMaxholdSecondaryRectanglesCall + smallRectanglesCall) : (rectanglesCall))+backgroundCall;
        const uint32_t timeUpdateCall = (rectanglesCall + dynamicMaxholdSecondaryRectanglesCall + backgroundCall);

        EXPECT_CALL(openGL, glClear(_)).Times(1);
        EXPECT_CALL(windowBase, getWindowSize).WillOnce(Return(WindowSize{1024,768}));
        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(timeUpdateCall+numberOfRectanglesCalls);
        EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(timeUpdateCall);

        EXPECT_CALL(openGL, glBindVertexArray(_)).Times(numberOfRectanglesCalls);

        EXPECT_CALL(openGL, glNamedBufferSubData(_,_,_,_)).Times((numberOfRectanglesCalls)).RetiresOnSaturation();
        EXPECT_CALL(openGL, glDrawArraysInstanced(_,_,_,_)).Times((numberOfRectanglesCalls));
        EXPECT_CALL(windowBase, getCursorPosition()).WillOnce(Return(CursorPosition{0,0}));
        EXPECT_CALL(windowBase, swapBuffers()).Times(1);
    }

    void expectDraw(const std::vector<float> &positions, const bool additionalRectanglesEnabled)
    {
        const uint32_t backgroundCall{1};
        const uint32_t numberOfRectangles = positions.size();
        const uint32_t numberOfSmallRectangles = positions.size();

        InSequence s;

        EXPECT_CALL(windowBase, getWindowSize).WillOnce(Return(WindowSize{1024,768}));
        EXPECT_CALL(openGL, glClear(_)).Times(1);

        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(rectanglesCall);
        EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(rectanglesCall);

        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(dynamicMaxholdSecondaryRectanglesCall);
        EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(dynamicMaxholdSecondaryRectanglesCall);

        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(backgroundCall);
        EXPECT_CALL(openGL, glProgramUniform1f(_,_,_)).Times(backgroundCall);

        EXPECT_CALL(openGL, glNamedBufferSubData(_,_,_,_)).Times(backgroundCall);
        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(backgroundCall);
        EXPECT_CALL(openGL, glBindVertexArray(_)).Times(backgroundCall);
        EXPECT_CALL(openGL, glDrawArraysInstanced(_,_,_,_)).Times(backgroundCall);

        EXPECT_CALL(windowBase, getCursorPosition()).WillOnce(Return(CursorPosition{0,0}));


        if(additionalRectanglesEnabled)
        {
            EXPECT_CALL(openGL, glNamedBufferSubData(_,_,_,_)).Times(dynamicMaxholdSecondaryRectanglesCall);
            EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(dynamicMaxholdSecondaryRectanglesCall);
            EXPECT_CALL(openGL, glBindVertexArray(_)).Times(dynamicMaxholdSecondaryRectanglesCall);
            EXPECT_CALL(openGL, glDrawArraysInstanced(_,_,_,_)).Times(dynamicMaxholdSecondaryRectanglesCall);
        }

        EXPECT_CALL(openGL, glNamedBufferSubData(_,_,_,_))
            .Times(rectanglesCall)
            .WillOnce([positions](GLuint, GLintptr, GLsizeiptr size, const void* data)
            {
                EXPECT_EQ(positions.size() * sizeof(Instance), size);

                const auto* instancesPtr = static_cast<const Instance*>(data);

                for(uint16_t i=0;i<positions.size();++i)
                {
                    EXPECT_NEAR(instancesPtr[i].y, positions.at(i), 0.001);
                }
            });

        EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(rectanglesCall);
        EXPECT_CALL(openGL, glBindVertexArray(_)).Times(rectanglesCall);
        EXPECT_CALL(openGL, glDrawArraysInstanced(_,_,_,_)).Times(rectanglesCall);

        if(additionalRectanglesEnabled)
        {
            EXPECT_CALL(openGL, glNamedBufferSubData(_,_,_,_)).Times(smallRectanglesCall);
            EXPECT_CALL(openGL, glBindProgramPipeline(_)).Times(smallRectanglesCall);
            EXPECT_CALL(openGL, glBindVertexArray(_)).Times(smallRectanglesCall);
            EXPECT_CALL(openGL, glDrawArraysInstanced(_,_,_,_)).Times(smallRectanglesCall);
        }

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

    void expectCheckIfThemeShouldBeChanged()
    {
        EXPECT_CALL(windowBase, checkIfThemeShouldBeChanged()).WillOnce(Return(std::nullopt));
    }

    void expectDestroyWindow()
    {
        EXPECT_CALL(openGL, glDeleteProgramPipelines(_,_)).Times(4);
        EXPECT_CALL(openGL, glDeleteProgram(_)).Times(8);
        EXPECT_CALL(text, finalize()).Times(1);
    }

};
