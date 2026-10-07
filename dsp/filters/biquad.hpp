#pragma once
#include <core/biquad_coefficients.hpp>
#include <core/constants.hpp>

template <typename T> class BiquadEngine {
public:
  BiquadEngine() { reset(); }

  void reset() { x1 = x2 = y1 = y2 = static_cast<T>(0.0); }

  void setCoefficients(const BiquadCoefficients<T> &newCoeffs) {
    coeffs = newCoeffs;
  }

  inline T processSample(T x) {
    T y = (coeffs.b0 * x) + (coeffs.b1 * x1) + (coeffs.b2 * x2) -
          (coeffs.a1 * y1) - (coeffs.a2 * y2);

    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;

    return y;
  }

private:
  BiquadCoefficients<T> coeffs;
  T x1 = 0.0, x2 = 0.0;
  T y1 = 0.0, y2 = 0.0;
};
