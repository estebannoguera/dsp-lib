#pragma once
#include <core/biquad_coefficients.hpp>

template <typename T> class BiquadDF2T {
private:
  T x1 = static_cast<T>(0.0), x2 = static_cast<T>(0.0);
  T y1 = static_cast<T>(0.0), y2 = static_cast<T>(0.0);
  T s1 = static_cast<T>(0.0), s2 = static_cast<T>(0.0);

  BiquadCoefficients<T> coeffs;

public:
  BiquadDF2T() { reset(); }
  void reset() { x1 = x2 = y1 = y2 = static_cast<T>(0.0); }

  void setCoefficients(const BiquadCoefficients<T> &newCoeffs) {
    coeffs = newCoeffs;
  }

  inline float processSample(float x) {

    float y = (coeffs.b0 * x) + s1;

    s1 = (coeffs.b1 * x) - (coeffs.a1 * y) + s2;
    s2 = (coeffs.b2 * x) - (coeffs.a2 * y);

    return y;
  }
};