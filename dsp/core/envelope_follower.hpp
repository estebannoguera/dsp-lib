#pragma once

template <typename T> class EnvelopeFollower {
public:
  EnvelopeFollower(T attack_time, T release_time)
      : attackCoefficient(attack_time), releaseCoefficient(release_time),
        envelope(0.0f) {}

  T processSample(T amplitude) {
    const T coefficient =
        amplitude > envelope ? attackCoefficient : releaseCoefficient;

    envelope += coefficient * (amplitude - envelope);

    return envelope;
  }

private:
  T attackCoefficient;
  T releaseCoefficient;
  T envelope;
};