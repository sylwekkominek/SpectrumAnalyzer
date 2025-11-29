/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include <gtest/gtest.h>
#include <cmath>

template <int indexOfMarginOfError>
struct ValuesChecker
{
    using Position = uint32_t;
    using ExpectedValue = float;

    template<typename T1=float, typename T2=float>
    static void valueChecker(const std::vector<T1> &expectedValues, const std::vector<T2> &values)
    {
        EXPECT_EQ(expectedValues.size(), values.size());
        EXPECT_EQ(true, std::equal(expectedValues.begin(), expectedValues.end(), values.begin(), [&](const auto &first, const auto &second){
                      return (std::abs(first - second) < marginOfError);
                  }));
    }

    static void valueChecker(const float expectedValue, const uint32_t expectedVectorSize, const std::vector<float> &values)
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

    static void positionValuesChecker(const std::map<Position,std::vector<ExpectedValue>> &positionExpecetedValues, const std::map<Position,std::vector<float>> &signals)
    {
        ASSERT_EQ(positionExpecetedValues.size(), signals.size());

        for(uint16_t i=0; i< signals.size(); ++i)
        {
            valueChecker(positionExpecetedValues.at(i), signals.at(i));
        }
    }

    static const float marginOfError;
};

template <int indexOfMarginOfError>
const float ValuesChecker<indexOfMarginOfError>::marginOfError = std::pow(10,indexOfMarginOfError);

