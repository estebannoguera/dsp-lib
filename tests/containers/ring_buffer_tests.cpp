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


    EXPECT_FLOAT_EQ(rb.getDelayed(0), 4.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(1), 3.0f);

    rb.write(5.0f);

    EXPECT_FLOAT_EQ(rb.getDelayed(0), 5.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(1), 4.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(2), 3.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(3), 2.0f);

    EXPECT_FLOAT_EQ(rb.getDelayed(1000), 0.0f);

}

TEST(RingBuffer, CapacityOneAlwaysStoresLatestSample)
{
    RingBuffer<int> rb(1);

    rb.write(10);
    EXPECT_EQ(rb.getDelayed(0), 10);

    rb.write(20);
    EXPECT_EQ(rb.getDelayed(0), 20);
}

TEST(RingBuffer, ZeroCapacityThrows) {
    EXPECT_THROW(RingBuffer<float>(0), std::invalid_argument);
}

TEST(RingBuffer, EmptyBufferAlwaysReturnsDefault) {
    RingBuffer<float> rb(8);

    EXPECT_FLOAT_EQ(rb.getDelayed(0), 0.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(1), 0.0f);
}


TEST(RingBufferTest, UnderstandGetDelayedSemantics)
{
    RingBuffer<int> buffer(4);

    buffer.write(10);
    buffer.write(20);
    buffer.write(30);

    EXPECT_EQ(buffer.getDelayed(0), 30);
    EXPECT_EQ(buffer.getDelayed(1), 20);
    EXPECT_EQ(buffer.getDelayed(2), 10);
}