#pragma once
#include "containers/ring_buffer.hpp"
#include "core/interpolators/linear_interpolator.hpp"
#include "core/delay_line_interpolation_helper.hpp"
#include <cmath>       // std::isfinite
#include <cstddef>     // size_t
#include <stdexcept>   // std::invalid_argument
#include <type_traits> // std::is_same_v

template <typename T, typename Interpolator = LinearInterpolator>

class DelayLine {
  static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>,
                "DelayLine requires float or double");

private:
  RingBuffer<T> buffer;
  double delaySamples;
  Interpolator interpolator;

public:
  DelayLine(size_t bufferSize, double delaySamples)
      : buffer(bufferSize), delaySamples(delaySamples), interpolator{} {
    if (!std::isfinite(delaySamples)) {
      throw std::invalid_argument("Delay must be finite");
    }
    if (delaySamples < 0) {
      throw std::invalid_argument("Delay must be positive");
    }
    if (delaySamples > bufferSize) {
      throw std::invalid_argument("Delay must not exceed buffer size");
    }
  }

  T processSample(T inputSample) {

    T delayed{};

    if (delaySamples > 0.0) {
      delayed = DelayLineInterpolationHelper<T, Interpolator>::process(
          buffer, interpolator, inputSample, delaySamples);
    }

    buffer.write(inputSample);

    return delaySamples == 0 ? inputSample : delayed;
  }

  void setDelaySamples(double delaySamples) {
    if (!std::isfinite(delaySamples)) {
      throw std::invalid_argument("Delay must be finite");
    }

    if (delaySamples > buffer.getSize()) {
      throw std::invalid_argument("Delay must not exceed buffer size");
    }
    if (delaySamples < 0) {
      throw std::invalid_argument("Delay must be positive");
    }
    this->delaySamples = delaySamples;
  }

  double getDelaySamples() const { return delaySamples; }
};