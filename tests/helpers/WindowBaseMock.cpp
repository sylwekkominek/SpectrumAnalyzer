/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "WindowBaseMock.hpp"
#include <functional>

std::function<void()> createWindowFunction;
std::function<bool()> checkIfWindowShouldBeClosedFunction;
std::function<bool()> checkIfWindowShouldBeRecreatedFunction;
std::function<std::optional<uint16_t>()> getUpdatedThemeNumberFunction;
std::function<void()> swapBuffersFunction;
std::function<CursorPosition()> getCursorPositionFunction;
std::function<WindowSize()> getWindowSizeFunction;

class WindowBase::WindowBaseImpl
{
public:
    WindowBaseImpl(const Configuration&, const bool)
    {
    }
    ~WindowBaseImpl()
    {
    }
};

WindowBase::WindowBase(const Configuration& config, const bool /*isFullScreenEnabled*/) :
    config(config)
{
}

void WindowBase::createWindow()
{
    createWindowFunction();
}

bool WindowBase::checkIfWindowShouldBeClosed()
{
    return checkIfWindowShouldBeClosedFunction();
}

bool WindowBase::checkIfWindowShouldBeRecreated()
{
    return checkIfWindowShouldBeRecreatedFunction();
}

std::optional<uint16_t> WindowBase::getUpdatedThemeNumber()
{
    return getUpdatedThemeNumberFunction();
}

void WindowBase::swapBuffers()
{
    swapBuffersFunction();
}

CursorPosition WindowBase::getCursorPosition()
{
    return getCursorPositionFunction();
}

WindowSize WindowBase::getWindowSize()
{
    return getWindowSizeFunction();
}

WindowBase::~WindowBase()
{
}

WindowBaseMock::WindowBaseMock() : WindowBase({}, false)
{
    createWindowFunction = [this]()
    {
        return this->createWindow();
    };

    checkIfWindowShouldBeClosedFunction = [this]()
    {
        return this->checkIfWindowShouldBeClosed();
    };

    checkIfWindowShouldBeRecreatedFunction = [this]()
    {
        return this->checkIfWindowShouldBeRecreated();
    };

    getUpdatedThemeNumberFunction = [this]()
    {
        return this->getUpdatedThemeNumber();
    };

    swapBuffersFunction = [this]()
    {
        return this->swapBuffers();
    };

    getCursorPositionFunction = [this]()
    {
        return this->getCursorPosition();
    };

    getWindowSizeFunction = [this]()
    {
        return this->getWindowSize();
    };
}

