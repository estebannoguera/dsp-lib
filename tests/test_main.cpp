#include <gtest/gtest.h>
#include "containers/ring_buffer.hpp"

TEST(RingBuffer, NewBufferIsEmpty)
{
    RingBuffer<float> rb(4);

    EXPECT_TRUE(rb.isEmpty());
    EXPECT_FALSE(rb.isFull());
    EXPECT_EQ(rb.getCount(), 0);
}

TEST(RingBuffer, NewBufferIsFull)
{
    RingBuffer<float> rb(4);
    rb.write(1.0f);
    rb.write(2.0f);
    rb.write(3.0f);
    rb.write(4.0f);

    EXPECT_TRUE(rb.isFull());
    EXPECT_FALSE(rb.isEmpty());
    EXPECT_EQ(rb.getCount(), 4);
}


TEST(RingBuffer, ReturnsCorrectHistory)
{

    RingBuffer<float> rb(4);
    rb.write(1.0f);
    rb.write(2.0f);
    rb.write(3.0f);
    rb.write(4.0f);


    EXPECT_EQ(rb.getDelayed(0), 4);
    EXPECT_EQ(rb.getDelayed(1), 3);

    rb.write(5.0f);

    EXPECT_EQ(rb.getDelayed(2), 3); 
    EXPECT_EQ(rb.getDelayed(3), 2); 

    EXPECT_EQ(rb.getDelayed(1000), 0.0f);

}

TEST(RingBuffer, ZeroCapacityThrows) {
    EXPECT_THROW(RingBuffer<float>(0), std::invalid_argument);
}

TEST(RingBuffer, NegativeCapacityThrows) {
    EXPECT_THROW(RingBuffer<float>(-1), std::invalid_argument);
}

TEST(RingBuffer, EmptyBufferAlwaysReturnsDefault) {
    RingBuffer<float> rb(8);

    EXPECT_FLOAT_EQ(rb.getDelayed(0), 0.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(1), 0.0f);
}

TEST(RingBuffer, GetDelayedWithNegativeSamplesAgoReturnsDefault) {
    RingBuffer<float> rb(8);
    rb.write(1.0f);
    rb.write(2.0f);

    EXPECT_FLOAT_EQ(rb.getDelayed(-1), 0.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(-5), 0.0f);
}