#include <gtest/gtest.h>
#include "modulation/chorus.hpp"

TEST(Chorus, PassesDrySignalWhenMixIsZero)
{
    Chorus<float> single(1, 44100.0f, 1.0f, 1, 0, 0.0f);
    Chorus<float> dual(2, 44100.0f, 1.0f, 1, 0, 0.0f);
    Chorus<float> tri(3, 44100.0f, 1.0f, 1, 0, 0.0f);

    EXPECT_FLOAT_EQ(single.processSample(0.5f), 0.5f);
    EXPECT_FLOAT_EQ(dual.processSample(0.5f), 0.5f);
    EXPECT_FLOAT_EQ(tri.processSample(0.5f), 0.5f);
}

TEST(Chorus, OneVoiceReturnsTheBaseDelayWhenDepthIsZero)
{
    Chorus<float> chorus(1, 44100.0f, 0.0f, 1, 0, 1.0f);

    EXPECT_FLOAT_EQ(chorus.processSample(1.0f), 0.0f);
    EXPECT_FLOAT_EQ(chorus.processSample(0.0f), 1.0f);
    EXPECT_FLOAT_EQ(chorus.processSample(0.0f), 0.0f);
}

TEST(Chorus, ExposesEachVoiceSoTheClientCanPan)
{
    Chorus<float> dual(2, 44100.0f, 0.0f, 1, 1, 1.0f);
    float voices[2] = {};

    dual.processSample(1.0f, voices);
    dual.processSample(0.0f, voices);

    float left = voices[0];
    float right = voices[1];

    EXPECT_FLOAT_EQ(left, 1.0f);
    EXPECT_FLOAT_EQ(right, 0.0f);
}

TEST(Chorus, RejectsZeroVoicesBaseDelayAndMixOutsideZeroToOne)
{
    EXPECT_THROW(Chorus<float>(0, 44100.0f, 1.0f, 1, 0, 0.5f), std::invalid_argument);
    EXPECT_THROW(Chorus<float>(1, 44100.0f, 1.0f, 0, 0, 0.5f), std::invalid_argument);
    EXPECT_THROW(Chorus<float>(1, 44100.0f, 1.0f, 1, 0, -0.1f), std::invalid_argument);
    EXPECT_THROW(Chorus<float>(1, 44100.0f, 1.0f, 1, 0, 1.1f), std::invalid_argument);
}
