#include "containers/ring_buffer.hpp"
#include <algorithm> // std::max
#include <cstddef>   // size_t
#include <stdexcept> // std::invalid_argument

template <typename T>

class DelayLine {

private:
  RingBuffer<T> buffer;
  size_t delaySamples;
  float feedback = 0.0f;
  float mix = 0.0f;

public:
  DelayLine(size_t bufferSize, size_t delaySamples, float feedback, float mix)
      : buffer(std::max(bufferSize, size_t(1))), delaySamples(delaySamples),
        feedback(feedback > 1   ? 1
                 : feedback < 0 ? 0
                                : feedback),
        mix(mix > 1   ? 1
            : mix < 0 ? 0
                      : mix) {
    if (feedback < 0 || feedback > 1) {
      throw std::invalid_argument("Feedback must be between 0 and 1");
    }
    if (mix < 0 || mix > 1) {
      throw std::invalid_argument("Mix must be between 0 and 1");
    }
  }

  T processSample(T inputSample) {

    if (delaySamples == 0) {
      return inputSample;
    }

    T delayed = buffer.getDelayed(delaySamples - 1);
    buffer.write(inputSample + delayed * feedback);
    return delayed * mix + inputSample * (1 - mix);
  }
};