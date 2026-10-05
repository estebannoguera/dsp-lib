#include <gtest/gtest.h>
#include "modulation/lfo.hpp"

TEST(LFO, SawReportsThePhaseAndWrapsAfterOneCycle)
{
    LFO<float> saw(4.0f, 1.0f, LFO<float>::Waveform::Saw);

    EXPECT_FLOAT_EQ(saw.tick(), 0.0f);
    EXPECT_FLOAT_EQ(saw.tick(), 0.25f);
    EXPECT_FLOAT_EQ(saw.tick(), 0.5f);
    EXPECT_FLOAT_EQ(saw.tick(), 0.75f);
    EXPECT_FLOAT_EQ(saw.tick(), 0.0f);
}

TEST(LFO, TriangleRisesToOneAndFallsBack)
{
    LFO<float> triangle(4.0f, 1.0f, LFO<float>::Waveform::Triangle);

    EXPECT_FLOAT_EQ(triangle.tick(), 0.0f);
    EXPECT_FLOAT_EQ(triangle.tick(), 0.5f);
    EXPECT_FLOAT_EQ(triangle.tick(), 1.0f);
    EXPECT_FLOAT_EQ(triangle.tick(), 0.5f);
    EXPECT_FLOAT_EQ(triangle.tick(), 0.0f);
}

TEST(LFO, SquareAndReverseSawFollowTheWrappedPhase)
{
    LFO<float> square(4.0f, 1.0f, LFO<float>::Waveform::Square);
    LFO<float> reverseSaw(4.0f, 1.0f, LFO<float>::Waveform::ReverseSaw);

    EXPECT_FLOAT_EQ(square.tick(), 1.0f);
    EXPECT_FLOAT_EQ(square.tick(), 1.0f);
    EXPECT_FLOAT_EQ(square.tick(), 0.0f);
    EXPECT_FLOAT_EQ(square.tick(), 0.0f);
    EXPECT_FLOAT_EQ(square.tick(), 1.0f);

    EXPECT_FLOAT_EQ(reverseSaw.tick(), 1.0f);
    EXPECT_FLOAT_EQ(reverseSaw.tick(), 0.75f);
    EXPECT_FLOAT_EQ(reverseSaw.tick(), 0.5f);
    EXPECT_FLOAT_EQ(reverseSaw.tick(), 0.25f);
    EXPECT_FLOAT_EQ(reverseSaw.tick(), 1.0f);
}

TEST(LFO, StartsAtTheGivenPhase)
{
    LFO<float> saw(4.0f, 1.0f, LFO<float>::Waveform::Saw, 0.25f);

    EXPECT_FLOAT_EQ(saw.tick(), 0.25f);
    EXPECT_FLOAT_EQ(saw.tick(), 0.5f);
}

TEST(LFO, RejectsPhaseOutsideZeroToOne)
{
    EXPECT_THROW(LFO<float>(4.0f, 1.0f, LFO<float>::Waveform::Saw, -0.1f),
                 std::invalid_argument);
    EXPECT_THROW(LFO<float>(4.0f, 1.0f, LFO<float>::Waveform::Saw, 1.0f),
                 std::invalid_argument);
}

TEST(LFO, HoldsThePhaseWhenFrequencyIsZero)
{
    LFO<float> saw(4.0f, 0.0f, LFO<float>::Waveform::Saw);

    EXPECT_FLOAT_EQ(saw.tick(), 0.0f);
    EXPECT_FLOAT_EQ(saw.tick(), 0.0f);
}

TEST(LFO, RejectsNonPositiveSampleRateAndNegativeFrequency)
{
    EXPECT_THROW(LFO<float>(0.0f), std::invalid_argument);
    EXPECT_THROW(LFO<float>(-48000.0f, 1.0f), std::invalid_argument);
    EXPECT_THROW(LFO<float>(44100.0f, -1.0f), std::invalid_argument);
}
