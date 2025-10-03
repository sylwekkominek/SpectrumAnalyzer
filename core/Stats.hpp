/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <deque>
#include <mutex>
#include <chrono>
#include <string>
#include <map>
#include <memory>

using namespace std::chrono;
using namespace std::chrono_literals;

class Stats
{
public:

    void update();

    uint32_t getNumberOfCallsInLast(std::chrono::microseconds numberOfMiliSeconds);

private:
    static constexpr auto maxNumberOfMilliSecondsKeptInStats{3000ms};
    std::deque<time_point<steady_clock>> queue;
};

class StatsManager
{
public:
    StatsManager(const std::string &name);
    void update();
    static void clear();
    static Stats getStatsFor(const std::string &name);

private:
    static std::unique_ptr<std::map<std::string,Stats>> statsPerName;
    static std::mutex queueMutex;
    const std::string name;
};


