#pragma once

#include <cmath>
#include <cstddef>
#include <type_traits>
#include "containers/ring_buffer.hpp"
#include "interpolators/linear_interpolator.hpp"

template <typename T, typename Interpolator>
struct DelayLineInterpolationHelper {
    static_assert(
        !std::is_same_v<T, T>,
        "Unsupported interpolator for DelayLine"
    );
};


template <typename T>
struct DelayLineInterpolationHelper<T, LinearInterpolator> {
  static T process(const RingBuffer<T> &buffer,
                   const LinearInterpolator &interpolator, T input,
                   double delaySamples) {
    const auto delay = static_cast<std::size_t>(std::floor(delaySamples));
    const double mu = delaySamples - static_cast<double>(delay);

    if (delaySamples < 1.0) {
      return interpolator.process(input, buffer.getDelayed(0), mu);
    }

    const T a = buffer.getDelayed(delay - 1);
    const T b = buffer.getDelayed(delay);

    return interpolator.process(a, b, mu);
  }
};