#pragma once

struct HermiteInterpolator {
    template <typename T>
    T process(T ym1, T y0, T y1, T y2, double mu) const {
        const double c0 = 2.0 * y0;
        const double c1 = -ym1 + y1;
        const double c2 = 2.0 * ym1 - 5.0 * y0
                        + 4.0 * y1 - y2;
        const double c3 = -ym1 + 3.0 * y0
                        - 3.0 * y1 + y2;

        return static_cast<T>(
            0.5 * (c0 + c1 * mu + c2 * mu * mu
                   + c3 * mu * mu * mu)
        );
    }
};