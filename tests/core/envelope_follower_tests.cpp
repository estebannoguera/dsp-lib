#include <gtest/gtest.h>
#include "core/envelope_follower.hpp"

TEST(EnvelopeFollowerTest, StartsAtZeroAndAttacks)
{
    EnvelopeFollower envelope(0.5f, 0.25f);

    EXPECT_FLOAT_EQ(envelope.processSample(0.8f), 0.4f);
}

TEST(EnvelopeFollowerTest, UsesAttackCoefficientWhenAmplitudeIncreases)
{
    EnvelopeFollower envelope(0.5f, 0.25f);

    envelope.processSample(0.2f);

    EXPECT_FLOAT_EQ(envelope.processSample(0.8f), 0.45f);
}

TEST(EnvelopeFollowerTest, UsesReleaseCoefficientWhenAmplitudeDecreases)
{
    EnvelopeFollower envelope(0.5f, 0.25f);

    envelope.processSample(0.8f);

    EXPECT_FLOAT_EQ(envelope.processSample(0.2f), 0.35f);
}

TEST(EnvelopeFollowerTest, DoesNotChangeWhenAmplitudeEqualsEnvelope)
{
    EnvelopeFollower envelope(0.5f, 0.25f);

    envelope.processSample(0.8f);
    envelope.processSample(0.2f);

    const float envelopeValue = envelope.processSample(0.35f);

    EXPECT_FLOAT_EQ(envelopeValue, 0.35f);
}

TEST(EnvelopeFollowerTest, ZeroCoefficientDoesNotChangeEnvelope)
{
    EnvelopeFollower envelope(0.0f, 0.0f);

    EXPECT_FLOAT_EQ(envelope.processSample(1.0f), 0.0f);
    EXPECT_FLOAT_EQ(envelope.processSample(0.5f), 0.0f);
}

TEST(EnvelopeFollowerTest, UnityCoefficientTracksImmediately)
{
    EnvelopeFollower envelope(1.0f, 1.0f);

    EXPECT_FLOAT_EQ(envelope.processSample(0.8f), 0.8f);
    EXPECT_FLOAT_EQ(envelope.processSample(0.2f), 0.2f);
}

TEST(EnvelopeFollowerTest, MaintainsStateBetweenSamples)
{
    EnvelopeFollower envelope(0.5f, 0.25f);

    EXPECT_FLOAT_EQ(envelope.processSample(1.0f), 0.5f);
    EXPECT_FLOAT_EQ(envelope.processSample(1.0f), 0.75f);
    EXPECT_FLOAT_EQ(envelope.processSample(1.0f), 0.875f);
}

TEST(EnvelopeFollowerTest, DoesNotOvershootInput)
{
    EnvelopeFollower envelope(0.5f, 0.25f);

    const float attack = envelope.processSample(1.0f);
    EXPECT_LE(attack, 1.0f);

    const float release = envelope.processSample(0.0f);
    EXPECT_GE(release, 0.0f);
}

