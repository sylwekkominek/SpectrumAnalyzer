/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/Stats.hpp"
#include <gtest/gtest.h>
#include <cmath>
#include <thread>


class StatsTests : public ::testing::Test
{
public:

    void firstThread()
    {
        const std::string processName{"firstThread"};
        StatsManager statsManager(processName);

        for(int i=0;i<numberOfCalls; ++i)
        {
            statsManager.update();
        }
    }

    void secondThread()
    {
        const std::string processName{"secondThread"};
        StatsManager statsManager(processName);

        for(int i=0;i<numberOfCalls; ++i)
        {
            statsManager.update();
        }
    }

    static constexpr int numberOfCalls{1000};
};

TEST_F(StatsTests, concurrencyCheck)
{
    std::thread thread1(std::thread(&StatsTests::firstThread,this));
    std::thread thread2(std::thread(&StatsTests::secondThread,this));

    thread1.join();
    thread2.join();

    EXPECT_EQ(numberOfCalls, StatsManager::getStatsFor("firstThread").getNumberOfCallsInLast(3000ms));
    EXPECT_EQ(numberOfCalls, StatsManager::getStatsFor("secondThread").getNumberOfCallsInLast(3000ms));
}

TEST_F(StatsTests, checkNumberOfCallsInLastTime)
{
    const int numberOfCalls{500};
    const int nextNumberOfCalls{754};

    const std::string processName{"test"};
    StatsManager statsManager(processName);

    for(int i=0;i<numberOfCalls; ++i)
    {
        statsManager.update();
    }

    EXPECT_EQ(numberOfCalls, StatsManager::getStatsFor("test").getNumberOfCallsInLast(1000ms));

    std::this_thread::sleep_for(1001ms);

    for(int i=0;i<nextNumberOfCalls; ++i)
    {
        statsManager.update();
    }

    EXPECT_EQ(nextNumberOfCalls, StatsManager::getStatsFor("test").getNumberOfCallsInLast(1000ms));
}
