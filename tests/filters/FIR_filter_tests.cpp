#include <gtest/gtest.h>
#include "filters/fir_filter.hpp"


TEST(FIRFilter, ThrowsOnEmptyCoefficients) {
    EXPECT_THROW(FIRFilter<float> filter({}), std::invalid_argument);
}

TEST(FIRFilter, ReturnsExpectedOutputForKnownSequence) {
    std::vector<float> coefficients = {0.25f, 0.25f, 0.25f, 0.25f};
    FIRFilter<float> filter(coefficients);

    EXPECT_FLOAT_EQ(filter.processSample(1.0f), 0.25f);
    EXPECT_FLOAT_EQ(filter.processSample(2.0f), 0.75f);
    EXPECT_FLOAT_EQ(filter.processSample(3.0f), 1.5f);
    EXPECT_FLOAT_EQ(filter.processSample(4.0f), 2.5f);
    EXPECT_FLOAT_EQ(filter.processSample(5.0f), 3.5f);
}