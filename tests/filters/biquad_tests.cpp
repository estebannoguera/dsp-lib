#include <gtest/gtest.h>
#include "filters/Biquad.hpp"

TEST(BiquadEngine, PassesInputWhenOnlyB0IsOne)
{
    BiquadEngine<float> filter;
    filter.setCoefficients({1.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    EXPECT_FLOAT_EQ(filter.processSample(1.0f), 1.0f);
    EXPECT_FLOAT_EQ(filter.processSample(2.0f), 2.0f);
}

TEST(BiquadEngine, B1DelaysTheInputByOneSample)
{
    BiquadEngine<float> filter;
    filter.setCoefficients({0.0f, 1.0f, 0.0f, 0.0f, 0.0f});

    EXPECT_FLOAT_EQ(filter.processSample(1.0f), 0.0f);
    EXPECT_FLOAT_EQ(filter.processSample(2.0f), 1.0f);
    EXPECT_FLOAT_EQ(filter.processSample(3.0f), 2.0f);
}

TEST(BiquadEngine, A1FeedsThePreviousOutputBack)
{
    BiquadEngine<float> filter;
    filter.setCoefficients({1.0f, 0.0f, 0.0f, -0.5f, 0.0f});

    EXPECT_FLOAT_EQ(filter.processSample(1.0f), 1.0f);
    EXPECT_FLOAT_EQ(filter.processSample(0.0f), 0.5f);
    EXPECT_FLOAT_EQ(filter.processSample(0.0f), 0.25f);
}

TEST(LPF, SettlesToOneForAConstantInput)
{
    BiquadEngine<float> filter;
    filter.setCoefficients(LPF<float>::calculate(48000.0f, 1000.0f, 0.707f));

    float output = 0.0f;
    for (int i = 0; i < 2000; ++i)
    {
        output = filter.processSample(1.0f);
    }

    EXPECT_NEAR(output, 1.0f, 0.0001f);
}