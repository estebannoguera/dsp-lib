#include <gtest/gtest.h>
#include "core/interpolators/hermite_interpolator.hpp"

TEST(HermiteInterpolator, ReturnsY0AtMuZero) {
    HermiteInterpolator interpolator;
    EXPECT_DOUBLE_EQ(interpolator.process(0.0, 10.0, 20.0, 30.0, 0.0),
                     10.0);
}

TEST(HermiteInterpolator, ReturnsY1AtMuOne) {
    HermiteInterpolator interpolator;
    EXPECT_DOUBLE_EQ(interpolator.process(0.0, 10.0, 20.0, 30.0, 1.0),
                     20.0);
}

TEST(HermiteInterpolator, ReproducesLinearRamp) {
    HermiteInterpolator interpolator;

    EXPECT_DOUBLE_EQ(
        interpolator.process(0.0, 10.0, 20.0, 30.0, 0.5),
        15.0
    );
}