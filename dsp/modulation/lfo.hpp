#pragma once

#include <cmath>
#include <stdexcept>

template <typename T> class LFO {

public:
  enum class Waveform { Sine, Triangle, Saw, ReverseSaw, Square };

  explicit LFO(T sampleRate, T frequency = static_cast<T>(1),
               Waveform waveform = Waveform::Sine,
               T phase = static_cast<T>(0))
      : sampleRate(sampleRate), frequency(frequency), phase(phase),
        waveform(waveform) {
    if (sampleRate <= static_cast<T>(0)) {
      throw std::invalid_argument("Sample rate must be positive");
    }
    if (frequency < static_cast<T>(0)) {
      throw std::invalid_argument("Frequency must be non-negative");
    }
    if (phase < static_cast<T>(0) || phase >= static_cast<T>(1)) {
      throw std::invalid_argument("Phase must be in the range [0, 1)");
    }
  }

  void prepare(double sampleRate);
  void reset();

  void setFrequency(float frequency);
  void setWaveform(Waveform waveform);

  T processSample(float input);

  T tick() {
    T phaseIncrement = frequency / sampleRate;
    T output = 0.0f;
    
    switch (waveform) {
    case Waveform::Sine:
      output = std::sin(phase);  
      break;
    case Waveform::Triangle:
      output = phase < 0.5f ? 2.0f * phase : 2.0f * (1.0f - phase);
      break;
    case Waveform::Saw:
      output = phase;
      break;
    case Waveform::ReverseSaw:
      output = 1.0f - phase;
      break;
    case Waveform::Square:
      output = phase < 0.5f ? 1.0f : 0.0f;
      break;
    }
    phase += phaseIncrement;
    if (phase >= static_cast<T>(1))
        phase -= static_cast<T>(1);
    return output;
  }

private:
  T sampleRate;
  T frequency;
  T phase = static_cast<T>(0);
  Waveform waveform;
};
