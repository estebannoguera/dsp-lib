#pragma once
#include <cmath>
#include <core/constants.hpp>

template <typename T>
struct BiquadCoefficients {
    T b0 = 0.0, b1 = 0.0, b2 = 0.0;
    T a1 = 0.0, a2 = 0.0;
};

template <typename T>
class LPF {
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
        return { b0 * invA0, b1 * invA0, b2 * invA0, a1 * invA0, a2 * invA0 };
    }
};

template <typename T>
class HPF {
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
        return { b0 * invA0, b1 * invA0, b2 * invA0, a1 * invA0, a2 * invA0 };
    }
};

template <typename T>
class BiquadEngine {
public:
    BiquadEngine() { reset(); }

    void reset() {
        x1 = x2 = y1 = y2 = static_cast<T>(0.0);
    }


    void setCoefficients(const BiquadCoefficients<T>& newCoeffs) {
        coeffs = newCoeffs;
    }


    inline T processSample(T x) {
        T y = (coeffs.b0 * x) + (coeffs.b1 * x1) + (coeffs.b2 * x2) - (coeffs.a1 * y1) - (coeffs.a2 * y2);

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
