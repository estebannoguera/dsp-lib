#pragma once

#include "envelope_follower.hpp"
#include "tools/utils.hpp"

template <typename T> class Compressor {
public:
  Compressor(T attackCoefficient, T releaseCoefficient, T thresholdDb, T ratio)
      : envelopeFollower(attackCoefficient, releaseCoefficient),
        thresholdDb(thresholdDb), ratio(ratio) {}

  T processSample(T input) {
    const T amplitude = std::abs(input);

    const T envelope = envelopeFollower.processSample(amplitude);

    if (envelope == T(0))
      return input;

    const T levelDb = amplitudeToDecibels(envelope);

    const T outputLevelDb = compressLevel(levelDb, thresholdDb, ratio);

    const T gainReductionDb = outputLevelDb - levelDb;

    const T gain = decibelsToLinear(gainReductionDb);

    return input * gain;
  }

private:
  EnvelopeFollower<T> envelopeFollower;

  T thresholdDb;
  T ratio;
};