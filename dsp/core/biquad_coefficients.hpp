#pragma once

template <typename T>
struct BiquadCoefficients {
    T b0 = 0.0, b1 = 0.0, b2 = 0.0;
    T a1 = 0.0, a2 = 0.0;
};