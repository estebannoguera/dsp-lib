#include "core/delay_line.hpp"
#include "core/interpolators/linear_interpolator.hpp"
#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>

struct FakeInterpolator {};

TEST(DelayLine, RejectsUnsupportedInterpolator) {
    DelayLine<float, FakeInterpolator> delay(4, 1.5);
}

TEST(DelayLine, AcceptsValidConstructorValues) {
  EXPECT_NO_THROW(DelayLine<float>(100, 0.0));
  EXPECT_NO_THROW((DelayLine<float, LinearInterpolator>(100, 1.0)));
  EXPECT_NO_THROW((DelayLine<float, LinearInterpolator>(100, 100.0)));
}

TEST(DelayLine, InterpolatesFractionalDelay) {
  DelayLine<float> delay(4, 1.5);

  EXPECT_FLOAT_EQ(delay.processSample(10.0f), 0.0f);
  EXPECT_FLOAT_EQ(delay.processSample(20.0f), 5.0f);
  EXPECT_FLOAT_EQ(delay.processSample(30.0f), 15.0f);
  EXPECT_FLOAT_EQ(delay.processSample(40.0f), 25.0f);

  delay.setDelaySamples(0.5);
  EXPECT_FLOAT_EQ(delay.processSample(10.0f), 25.0f);
  EXPECT_FLOAT_EQ(delay.processSample(20.0f), 15.0f);

  delay.setDelaySamples(0.0);

  EXPECT_FLOAT_EQ(delay.processSample(30.0f), 30.0f);
  EXPECT_FLOAT_EQ(delay.processSample(40.0f), 40.0f);

  delay.setDelaySamples(0.5);
  EXPECT_FLOAT_EQ(delay.processSample(50.0f), 45.0f);
}

TEST(DelayLine, RejectsInvalidConstructorValues) {
  EXPECT_THROW(DelayLine<float>(100, -1.0), std::invalid_argument);
  EXPECT_THROW(DelayLine<float>(100, 101.0), std::invalid_argument);
  EXPECT_THROW(DelayLine<float>(100, std::numeric_limits<double>::infinity()),
               std::invalid_argument);
  EXPECT_THROW(DelayLine<float>(100, std::numeric_limits<double>::quiet_NaN()),
               std::invalid_argument);
}

TEST(DelayLine, AcceptsValidDelayChanges) {
  DelayLine<float> delay(100, 2.0);

  EXPECT_NO_THROW(delay.setDelaySamples(0.0));
  EXPECT_DOUBLE_EQ(delay.getDelaySamples(), 0.0);

  EXPECT_NO_THROW(delay.setDelaySamples(50.0));
  EXPECT_DOUBLE_EQ(delay.getDelaySamples(), 50.0);

  EXPECT_NO_THROW(delay.setDelaySamples(100.0));
  EXPECT_DOUBLE_EQ(delay.getDelaySamples(), 100.0);

  EXPECT_NO_THROW(delay.setDelaySamples(2.5));
  EXPECT_DOUBLE_EQ(delay.getDelaySamples(), 2.5);
}

TEST(DelayLine, RejectsInvalidDelayAndPreservesPreviousValue) {
  DelayLine<float> delay(100, 2.0);

  EXPECT_THROW(delay.setDelaySamples(-1.0), std::invalid_argument);
  EXPECT_DOUBLE_EQ(delay.getDelaySamples(), 2.0);

  EXPECT_THROW(delay.setDelaySamples(101.0), std::invalid_argument);
  EXPECT_DOUBLE_EQ(delay.getDelaySamples(), 2.0);

  EXPECT_THROW(delay.setDelaySamples(std::numeric_limits<double>::infinity()),
               std::invalid_argument);
  EXPECT_DOUBLE_EQ(delay.getDelaySamples(), 2.0);

  EXPECT_THROW(delay.setDelaySamples(std::numeric_limits<double>::quiet_NaN()),
               std::invalid_argument);
  EXPECT_DOUBLE_EQ(delay.getDelaySamples(), 2.0);
}

TEST(DelayLine, ZeroDelayOutputsInputAndPreservesHistory) {
  DelayLine<float> delay(4, 0.0);

  EXPECT_FLOAT_EQ(delay.processSample(10.0f), 10.0f);
  EXPECT_FLOAT_EQ(delay.processSample(20.0f), 20.0f);

  delay.setDelaySamples(2.0);

  EXPECT_FLOAT_EQ(delay.processSample(30.0f), 10.0f);
}

TEST(DelayLine, DelaysSamplesByConfiguredAmount) {
  DelayLine<float> delay(4, 2.0);

  EXPECT_FLOAT_EQ(delay.processSample(10.0f), 0.0f);
  EXPECT_FLOAT_EQ(delay.processSample(20.0f), 0.0f);
  EXPECT_FLOAT_EQ(delay.processSample(30.0f), 10.0f);
  EXPECT_FLOAT_EQ(delay.processSample(40.0f), 20.0f);
}

TEST(DelayLine, SupportsMaximumDelay) {
  DelayLine<float> delay(3, 3.0);

  EXPECT_FLOAT_EQ(delay.processSample(10.0f), 0.0f);
  EXPECT_FLOAT_EQ(delay.processSample(20.0f), 0.0f);
  EXPECT_FLOAT_EQ(delay.processSample(30.0f), 0.0f);
  EXPECT_FLOAT_EQ(delay.processSample(40.0f), 10.0f);
}

TEST(DelayLine, ChangesDelayWithoutResettingHistory) {
  DelayLine<float> delay(4, 1.0);

  EXPECT_FLOAT_EQ(delay.processSample(10.0f), 0.0f);
  EXPECT_FLOAT_EQ(delay.processSample(20.0f), 10.0f);

  delay.setDelaySamples(2.0);

  EXPECT_FLOAT_EQ(delay.processSample(30.0f), 10.0f);
  EXPECT_FLOAT_EQ(delay.processSample(40.0f), 20.0f);
}

TEST(DelayLine, SwitchingToZeroDelayPreservesHistory) {
  DelayLine<float, LinearInterpolator> delay(4, 2.0);

  EXPECT_FLOAT_EQ(delay.processSample(10.0f), 0.0f);
  EXPECT_FLOAT_EQ(delay.processSample(20.0f), 0.0f);

  delay.setDelaySamples(0.0);
  EXPECT_FLOAT_EQ(delay.processSample(30.0f), 30.0f);

  delay.setDelaySamples(2.0);
  EXPECT_FLOAT_EQ(delay.processSample(40.0f), 20.0f);
}

TEST(DelayLine, RejectsZeroBufferCapacity) {
  EXPECT_THROW(DelayLine<float>(0, 0.0), std::invalid_argument);
}