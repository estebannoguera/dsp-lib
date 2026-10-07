#include "core/compressor.hpp"
#include "gtest/gtest.h"

TEST(CompressorTest, CompressesSampleAboveThreshold) {
  Compressor<double> compressor(1.0, 1.0, -10.0, 2.0);

  const double output = compressor.processSample(0.5);

  EXPECT_NEAR(output, 0.3976353644, 1e-6);

}

TEST(CompressorTest, CompressesSampleBelowThreshold) {
    Compressor<double> compressor(1.0, 1.0, -10.0, 2.0);
  
    const double output = compressor.processSample(0.25);
  
    EXPECT_DOUBLE_EQ(output, 0.25);
  
  }

  TEST(CompressorTest, DoesNotCompressAtThreshold)
{
    Compressor<double> compressor(
        1.0,
        1.0,
        -10.0,
        2.0
    );

    const double input = std::pow(10.0, -10.0 / 20.0);

    const double output = compressor.processSample(input);

    EXPECT_NEAR(output, input, 1e-6);
}