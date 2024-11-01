/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include <gtest/gtest.h>
#include <cmath>

template <int indexOfMarginOfError>
struct ValuesChecker
{
    using Position = uint;
    using ExpectedValue = float;

    static void valueChecker(const std::vector<float> &expectedValues, const std::vector<float> &values)
    {
        EXPECT_EQ(expectedValues.size(), values.size());
        EXPECT_EQ(true, std::equal(expectedValues.begin(), expectedValues.end(), values.begin(), [&](const auto &first, const auto &second){
            return (std::abs(first - second) < marginOfError);
        }));
    }

    static void valueChecker(const float expectedValue, const uint expectedVectorSize, const std::vector<float> &values)
    {
        EXPECT_EQ(expectedVectorSize, values.size());

        for(const auto &el : values)
        {
            EXPECT_NEAR(el, expectedValue, marginOfError);
        }
    }

    static void positionValueChecker(const std::map<Position,ExpectedValue> &positionExpecetedValue, const std::vector<float> &signal)
    {
        for(const auto&[position, value]: positionExpecetedValue)
        {
            EXPECT_NEAR(signal[position], value, marginOfError);
        }
    }

    static constexpr float marginOfError = exp(indexOfMarginOfError);
};
