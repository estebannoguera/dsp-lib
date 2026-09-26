#include <gtest/gtest.h>
#include "core/delay_line.hpp"

TEST(DelayLine, MixBlendsDryAndDelayedSignal)
{
    DelayLine<float> delay(1, 1, 0.0f, 0.5f);

    EXPECT_FLOAT_EQ(delay.processSample(1.0f), 0.5f);
    EXPECT_FLOAT_EQ(delay.processSample(0.0f), 0.5f);
    EXPECT_FLOAT_EQ(delay.processSample(0.0f), 0.0f);
}

TEST(DelayLine, FeedbackRecirculatesIntoTheBuffer)
{
    DelayLine<float> delay(1, 1, 0.5f, 1.0f);

    EXPECT_FLOAT_EQ(delay.processSample(1.0f), 0.0f);
    EXPECT_FLOAT_EQ(delay.processSample(0.0f), 1.0f);
    EXPECT_FLOAT_EQ(delay.processSample(0.0f), 0.5f);
    EXPECT_FLOAT_EQ(delay.processSample(0.0f), 0.25f);
}

TEST(DelayLine, RejectsFeedbackAndMixOutsideZeroToOne)
{
    EXPECT_THROW(DelayLine<float>(1, 1, -0.1f, 0.5f), std::invalid_argument);
    EXPECT_THROW(DelayLine<float>(1, 1, 1.1f, 0.5f), std::invalid_argument);
    EXPECT_THROW(DelayLine<float>(1, 1, 0.5f, -0.1f), std::invalid_argument);
    EXPECT_THROW(DelayLine<float>(1, 1, 0.5f, 1.1f), std::invalid_argument);
}