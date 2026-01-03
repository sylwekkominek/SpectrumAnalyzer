/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template<typename T>
class DataExchanger
{
public:
    DataExchanger(uint32_t maxQueueSize);
    void push_back(T &&value);
    T get();
    std::optional<T> getWithoutBlocking();
    void stop();
    uint32_t getSize();

private:

    std::mutex queueMutex;
    std::condition_variable queueConditionVariable;
    std::queue<T> queue;
    uint32_t maxQueueSize;
};

template<typename T>
DataExchanger<T>::DataExchanger(uint32_t maxQueueSize):
    maxQueueSize(maxQueueSize)
{
}

template<typename T>
void DataExchanger<T>::push_back(T &&value)
{
    std::lock_guard<std::mutex> lg(queueMutex);

    queue.push(std::move(value));

    if(queue.size()>maxQueueSize)
    {
        while(not queue.empty())
        {
            queue.pop();
        }
    }

    queueConditionVariable.notify_one();
}

template<typename T>
T DataExchanger<T>::get()
{
    std::unique_lock<std::mutex> ul(queueMutex);
    queueConditionVariable.wait(ul,[this](){return not this->queue.empty();});
    auto value = std::move(queue.front());
    queue.pop();

    return value;
}

template<typename T>
std::optional<T> DataExchanger<T>::getWithoutBlocking()
{
    std::unique_lock<std::mutex> ul(queueMutex);

    if(not queue.empty())
    {
        auto value = std::move(queue.front());
        queue.pop();
        return value;
    }

    return {};
}

template<typename T>
void DataExchanger<T>::stop()
{
    std::lock_guard<std::mutex> lg(queueMutex);
    queue.push(std::move(T{}));
    queueConditionVariable.notify_one();
}

template<typename T>
uint32_t DataExchanger<T>::getSize()
{
    std::unique_lock<std::mutex> ul(queueMutex);
    return queue.size();
}
