#pragma once
#include <cmath>
#include <core/biquad_coefficients.hpp>
#include <core/constants.hpp>

template <typename T> class LPF {
public:
  static BiquadCoefficients<T> calculate(T sampleRate, T f0, T Q) {
    T omega = static_cast<T>(TWO_PI) * f0 / sampleRate;
    T cosW = std::cos(omega);
    T alpha = std::sin(omega) / (static_cast<T>(2.0) * Q);

    T b0 = (static_cast<T>(1.0) - cosW) / static_cast<T>(2.0);
    T b1 = static_cast<T>(1.0) - cosW;
    T b2 = b0;
    T a0 = static_cast<T>(1.0) + alpha;
    T a1 = static_cast<T>(-2.0) * cosW;
    T a2 = static_cast<T>(1.0) - alpha;

    T invA0 = static_cast<T>(1.0) / a0;
    return {b0 * invA0, b1 * invA0, b2 * invA0, a1 * invA0, a2 * invA0};
  }
};

template <typename T> class HPF {
public:
  static BiquadCoefficients<T> calculate(T sampleRate, T f0, T Q) {
    T omega = static_cast<T>(TWO_PI) * f0 / sampleRate;
    T cosW = std::cos(omega);
    T alpha = std::sin(omega) / (static_cast<T>(2.0) * Q);

    T b0 = (static_cast<T>(1.0) + cosW) / static_cast<T>(2.0);
    T b1 = -(static_cast<T>(1.0) + cosW);
    T b2 = b0;
    T a0 = static_cast<T>(1.0) + alpha;
    T a1 = static_cast<T>(-2.0) * cosW;
    T a2 = static_cast<T>(1.0) - alpha;

    T invA0 = static_cast<T>(1.0) / a0;
    return {b0 * invA0, b1 * invA0, b2 * invA0, a1 * invA0, a2 * invA0};
  }
};