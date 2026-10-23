#pragma once

/**
 * @brief Linear interpolator
 * 
 * @tparam T The type of the input and output values
 * @param a The first value more recent sample
 * @param b The second value older sample
 * @param mu The interpolation factor
 * @return The interpolated value
 */
struct LinearInterpolator {
    template <typename T>
    T process(T a, T b, double mu) const {
        return a * (1.0 - mu) + b * mu;
    }
};