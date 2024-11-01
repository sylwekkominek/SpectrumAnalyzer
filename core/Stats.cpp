/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Stats.hpp"

std::unique_ptr<std::map<std::string,Stats>> StatsManager::statsPerName{std::make_unique<std::map<std::string,Stats>>()};
std::mutex StatsManager::queueMutex{};

void Stats::update()
{
    const auto timeNow = high_resolution_clock::now();
    queue.push_back(timeNow);

    while(!queue.empty())
    {
        if((timeNow - queue.front()) > maxNumberOfMilliSecondsKeptInStats)
        {
            queue.pop_front();
        }
        else
        {
            break;
        }
    }
}

uint32_t Stats::getNumberOfCallsInLast(std::chrono::microseconds numberOfMiliSeconds)
{
    uint i{0};


    for(auto el = queue.rbegin(); el != queue.rend(); ++el)
    {
        if(duration_cast<milliseconds>(*queue.rbegin() - *el ) < numberOfMiliSeconds)
        {
            ++i;
        }
        else
        {
            break;
        }
    }

    return i;
}


StatsManager::StatsManager(const std::string &name): name(name)
{
    std::lock_guard<std::mutex> lg(queueMutex);
    statsPerName->insert(std::make_pair(name,Stats{}));
}

void StatsManager::update()
{
    std::lock_guard<std::mutex> lg(queueMutex);
    statsPerName->at(name).update();
}

void StatsManager::clear()
{
    std::lock_guard<std::mutex> lg(queueMutex);
    statsPerName->clear();
}

Stats StatsManager::getStatsFor(const std::string &name)
{
    std::lock_guard<std::mutex> lg(queueMutex);
    return statsPerName->at(name);
}


