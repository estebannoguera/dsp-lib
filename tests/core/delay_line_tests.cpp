#include <gtest/gtest.h>
#include "core/delay_line.hpp"

TEST(DelayLine, WorksWithDifferentTypes)
{
    {
        DelayLine<int> delay(3);

        EXPECT_EQ(delay.processSample(1), 0);
        EXPECT_EQ(delay.processSample(2), 0);
        EXPECT_EQ(delay.processSample(3), 0);
        EXPECT_EQ(delay.processSample(4), 1);
    }

    {
        DelayLine<float> delay(3);

        EXPECT_FLOAT_EQ(delay.processSample(1.0f), 0.0f);
        EXPECT_FLOAT_EQ(delay.processSample(2.0f), 0.0f);
        EXPECT_FLOAT_EQ(delay.processSample(3.0f), 0.0f);
        EXPECT_FLOAT_EQ(delay.processSample(4.0f), 1.0f);
    }
}

TEST(DelayLineTest, ZeroDelayActsAsIdentity)
{
    DelayLine<int> delay(0);

    EXPECT_EQ(delay.processSample(1), 1);
    EXPECT_EQ(delay.processSample(2), 2);
    EXPECT_EQ(delay.processSample(3), 3);

}