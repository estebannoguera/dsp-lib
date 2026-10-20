#include <gtest/gtest.h>
#include "containers/ring_buffer.hpp"

TEST(RingBuffer, NewBufferIsEmpty)
{
    RingBuffer<float> rb(4);

    EXPECT_TRUE(rb.isEmpty());
    EXPECT_FALSE(rb.isFull());
    EXPECT_EQ(rb.getCount(), 0);
    EXPECT_FLOAT_EQ(rb.getDelayed(0), 0.0f);
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
    
    EXPECT_FLOAT_EQ(rb.getDelayed(0), 0.0f);

    rb.write(1.0f);
    rb.write(2.0f);
    rb.write(3.0f);
    rb.write(4.0f);


    EXPECT_FLOAT_EQ(rb.getDelayed(0), 4.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(1), 3.0f);

    rb.write(5.0f);
    rb.write(6.0f);

    EXPECT_FLOAT_EQ(rb.getDelayed(0), 6.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(1), 5.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(2), 4.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(3), 3.0f);

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
    EXPECT_FLOAT_EQ(rb.getDelayed(2), 0.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(3), 0.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(4), 0.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(5), 0.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(6), 0.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(7), 0.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(8), 0.0f);
    EXPECT_FLOAT_EQ(rb.getDelayed(9), 0.0f);

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
    EXPECT_EQ(buffer.getDelayed(3), 0);
}


TEST(RingBufferTest, MoveTransfersContents) {
    RingBuffer<float> a(3); a.write(1.f); a.write(2.f);
    RingBuffer<float> b = std::move(a);
    EXPECT_EQ(b.getDelayed(0), 2.f);
    EXPECT_EQ(b.getCount(), 2u);
    EXPECT_TRUE(a.isEmpty());
  }
  
  static_assert(std::is_nothrow_move_constructible_v<RingBuffer<float>>);
  static_assert(!std::is_copy_constructible_v<RingBuffer<float>>);
  static_assert(!std::is_convertible_v<size_t, RingBuffer<float>>); 