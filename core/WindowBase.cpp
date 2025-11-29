/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "WindowBase.hpp"
#include <GLFW/glfw3.h>
#include <atomic>


class WindowBase::WindowBaseImpl
{
public:
    WindowBaseImpl(const Configuration& config, const bool fullScreenEnabled);
    void createWindow();
    bool checkIfWindowShouldBeClosed();
    bool checkIfWindowShouldBeRecreated();
    void swapBuffers();
    CursorPosition getCursorPosition();
    WindowSize getWindowSize();
    ~WindowBaseImpl();

private:

    static std::atomic<WindowSize> windowSize;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void cursorEnteredAreaOverWindowCallback(GLFWwindow* window, int entered);
    static void updateCurrentWindowSize(const uint16_t x, const uint16_t y);

    const Configuration &config;
    bool isFullScreenEnabled;
    GLFWwindow* window;
};

std::atomic<WindowSize> WindowBase::WindowBaseImpl::windowSize = {};

WindowBase::WindowBaseImpl::WindowBaseImpl(const Configuration &config, const bool isFullScreenEnabled) :
    config(config),
    isFullScreenEnabled(isFullScreenEnabled)
{
}

void WindowBase::WindowBaseImpl::createWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const auto name = "Not Yet Another Spectrum Analyzer";

    if (isFullScreenEnabled)
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        const auto maximizedWindowSize = config.get<MaximizedWindowSize>();
        window = glfwCreateWindow(maximizedWindowSize.first, maximizedWindowSize.second, name, glfwGetPrimaryMonitor(), nullptr);
        updateCurrentWindowSize(maximizedWindowSize.first, maximizedWindowSize.second);

    }
    else
    {
        const auto normalWindowSize = config.get<NormalWindowSize>();
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        window = glfwCreateWindow(normalWindowSize.first, normalWindowSize.second, name, nullptr, nullptr);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        updateCurrentWindowSize(normalWindowSize.first, normalWindowSize.second);
    }

    glfwSetCursorEnterCallback(window, cursorEnteredAreaOverWindowCallback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

bool WindowBase::WindowBaseImpl::checkIfWindowShouldBeClosed()
{
    glfwPollEvents();
    return glfwWindowShouldClose(window);
}

bool WindowBase::WindowBaseImpl::checkIfWindowShouldBeRecreated()
{
    glfwPollEvents();

    if((isFullScreenEnabled && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
    {
        return true;
    }

    if((not isFullScreenEnabled) && glfwGetWindowAttrib(window, GLFW_MAXIMIZED))
    {
        return true;
    }

    return false;
}

void WindowBase::WindowBaseImpl::framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
    updateCurrentWindowSize(width, height);
}

void WindowBase::WindowBaseImpl::cursorEnteredAreaOverWindowCallback(GLFWwindow* window, int entered)
{
    glfwSetInputMode(window, GLFW_CURSOR, (entered ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL));
}

WindowBase::WindowBaseImpl::~WindowBaseImpl()
{
    glfwDestroyWindow(window);
}

void WindowBase::WindowBaseImpl::swapBuffers()
{
    glfwSwapBuffers(window);
}

void WindowBase::WindowBaseImpl::updateCurrentWindowSize(const uint16_t x, const uint16_t y)
{
    windowSize = {x,y};
}

CursorPosition WindowBase::WindowBaseImpl::getCursorPosition()
{
    CursorPosition cursorPosition{};
    glfwGetCursorPos(window, &cursorPosition.x, &cursorPosition.y);
    return cursorPosition;
}

WindowSize WindowBase::WindowBaseImpl::getWindowSize()
{
    return windowSize;
}

WindowBase::WindowBase(const Configuration& config, const bool isFullScreenEnabled) :
    config(config),
    isFullScreenEnabled(isFullScreenEnabled),
    windowBaseImpl(std::make_unique<WindowBaseImpl>(config, isFullScreenEnabled))
{
}

void WindowBase::createWindow()
{
    windowBaseImpl->createWindow();
}

bool WindowBase::checkIfWindowShouldBeClosed()
{
    return windowBaseImpl->checkIfWindowShouldBeClosed();
}

bool WindowBase::checkIfWindowShouldBeRecreated()
{
    return windowBaseImpl->checkIfWindowShouldBeRecreated();
}

WindowBase::~WindowBase()
{
    windowBaseImpl = nullptr;
}

void WindowBase::swapBuffers()
{
    windowBaseImpl->swapBuffers();
}

CursorPosition WindowBase::getCursorPosition()
{
    return windowBaseImpl->getCursorPosition();
}

WindowSize WindowBase::getWindowSize()
{
    return windowBaseImpl->getWindowSize();
}
