#include "gtest/gtest.h"
#include "tools/utils.hpp"

TEST(UtilsTest, ConvertsUnitAmplitudeToZeroDecibels)
{
    EXPECT_FLOAT_EQ(amplitudeToDecibels(1.0f), 0.0f);
}

TEST(UtilsTest, ConvertsHalfAmplitudeToDecibels)
{
    EXPECT_NEAR(amplitudeToDecibels(0.5f), -6.0206f, 0.0001f);
}

TEST(UtilsTest, ConvertsTenthAmplitudeToDecibels)
{
    EXPECT_NEAR(amplitudeToDecibels(0.1f), -20.0f, 0.0001f);
}

TEST(UtilsTest, ZeroAmplitudeReturnsNegativeInfinity)
{
    const float result = amplitudeToDecibels(0.0f);

    EXPECT_TRUE(std::isinf(result));
    EXPECT_LT(result, 0.0f);
}


TEST(CompressLevelTest, BelowThreshold)
{
    EXPECT_DOUBLE_EQ(
        compressLevel(-15.0, -10.0, 2.0),
        -15.0
    );
}

TEST(CompressLevelTest, AtThreshold)
{
    EXPECT_DOUBLE_EQ(
        compressLevel(-10.0, -10.0, 2.0),
        -10.0
    );
}

TEST(CompressLevelTest, AboveThreshold)
{
    EXPECT_DOUBLE_EQ(
        compressLevel(-6.0, -10.0, 2.0),
        -8.0
    );
}

TEST(DecibelsToLinearTest, ZeroDecibels)
{
    EXPECT_DOUBLE_EQ(
        decibelsToLinear(0.0),
        1.0
    );
}

TEST(DecibelsToLinearTest, MinusSixDecibels)
{
    EXPECT_NEAR(
        decibelsToLinear(-6.0),
        0.501187,
        1e-6
    );
}

TEST(DecibelsToLinearTest, MinusTwentyDecibels)
{
    EXPECT_DOUBLE_EQ(
        decibelsToLinear(-20.0),
        0.1
    );
}

TEST(DecibelsToLinearTest, MinusInfinity)
{
    const double result =
        decibelsToLinear(-std::numeric_limits<double>::infinity());

    EXPECT_EQ(result, 0.0);
}