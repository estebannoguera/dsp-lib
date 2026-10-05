#include "containers/ring_buffer.hpp"
#include "modulation/lfo.hpp"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

template<typename T>

class Chorus {

    private:
        std::vector<LFO<T>> lfos;
        std::vector<std::unique_ptr<RingBuffer<T>>> delays;
        size_t baseDelaySamples;
        size_t depthSamples;
        T mix;

    public:
        Chorus(size_t voiceCount, T sampleRate, T frequency, size_t baseDelaySamples, size_t depthSamples, T mix)
            : baseDelaySamples(baseDelaySamples),
            depthSamples(depthSamples),
            mix(mix)
        {
            if (voiceCount == 0)
            {
                throw std::invalid_argument("Voice count must be at least 1");
            }

            if (baseDelaySamples == 0)
            {
                throw std::invalid_argument("Base delay must be at least 1 sample");
            }

            if (mix < 0 || mix > 1)
            {
                throw std::invalid_argument("Mix must be between 0 and 1");
            }

            lfos.reserve(voiceCount);
            delays.reserve(voiceCount);

            size_t bufferSize = baseDelaySamples + depthSamples;

            for (size_t voice = 0; voice < voiceCount; ++voice)
            {
                T phase = static_cast<T>(voice) / static_cast<T>(voiceCount);
                lfos.emplace_back(sampleRate, frequency, LFO<T>::Waveform::Triangle, phase);
                delays.push_back(std::make_unique<RingBuffer<T>>(bufferSize));
            }
        }

        T processSample(T inputSample, T* voiceOutputs = nullptr) {

            T wet = T{};

            for (size_t voice = 0; voice < lfos.size(); ++voice)
            {
                size_t delaySamples = baseDelaySamples + static_cast<size_t>(lfos[voice].tick() * static_cast<T>(depthSamples));
                T voiceSample = delays[voice]->getDelayed(delaySamples - 1);
                delays[voice]->write(inputSample);

                if (voiceOutputs != nullptr)
                {
                    voiceOutputs[voice] = voiceSample;
                }

                wet += voiceSample;
            }

            wet /= static_cast<T>(lfos.size());

            return inputSample * (static_cast<T>(1) - mix) + wet * mix;
        }

};
