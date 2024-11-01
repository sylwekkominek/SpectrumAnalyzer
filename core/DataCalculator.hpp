/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <algorithm>
#include <deque>
#include <functional>

template<typename T>
class DataCalculatorBase
{
public:

    DataCalculatorBase(uint numberOfSamples, uint numberOfSignalsForProcessing);
    void push_back(const std::vector<T>&data);
    virtual void updateAlgorithm()=0;
    std::vector<T> calculateWithMoving();
    std::vector<T> calculateWithClearing();
    void clear();

protected:

    std::function<T(T&,T&)> algorithm;
    uint numberOfSignalsForProcessing;

private:

    std::deque<std::vector<T>> dataQueue;
    uint numberOfSamples;
};


template<typename T>
DataCalculatorBase<T>::DataCalculatorBase(uint numberOfSamples, uint numberOfSignalsForProcessing):
    numberOfSamples(numberOfSamples),
    numberOfSignalsForProcessing(numberOfSignalsForProcessing)
{
}

template<typename T>
void DataCalculatorBase<T>::push_back(const std::vector<T>&data)
{
    dataQueue.push_back(data);
}

template<typename T>
std::vector<T> DataCalculatorBase<T>::calculateWithMoving()
{

    if(dataQueue.size() >= numberOfSignalsForProcessing)
    {
        std::vector<T> data(numberOfSamples,0);

        for(uint i =0;i<numberOfSignalsForProcessing;++i)
        {
            std::transform(data.begin(), data.end(), dataQueue[i].begin(), data.begin(), algorithm);
        }

        dataQueue.pop_front();
        return data;
    }
    return {};
}

template<typename T>
std::vector<T> DataCalculatorBase<T>::calculateWithClearing()
{

    if(dataQueue.size() >= numberOfSignalsForProcessing)
    {
        std::vector<T> data(numberOfSamples,0);

        for(uint i =0;i<numberOfSignalsForProcessing;++i)
        {
            std::transform(data.begin(), data.end(), dataQueue[i].begin(), data.begin(), algorithm);
        }

        dataQueue.clear();
        return data;
    }
    return {};
}

template<typename T>
void DataCalculatorBase<T>::clear()
{
    dataQueue.clear();
}


class DataAverager : public DataCalculatorBase<float>
{
public:
    DataAverager(uint numberOfSamples, uint numberOfSignalsForProcessing);
private:
    void updateAlgorithm() override;
};

class DataMaxHolder : public DataCalculatorBase<float>
{
public:
    DataMaxHolder(uint numberOfSamples, uint numberOfSignalsForProcessing);

private:
    void updateAlgorithm() override;
};


