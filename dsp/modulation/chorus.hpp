#include "containers/ring_buffer.hpp"
#include "modulation/lfo.hpp"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

template<typename T>

class Chorus {

    public:
        enum class ChorusType { ChorusEngine = 1, DualChorus = 2, TriChorus = 3 };

    private:
        std::vector<LFO<T>> lfos;
        std::vector<std::unique_ptr<RingBuffer<T>>> delays;
        size_t baseDelaySamples;
        size_t depthSamples;
        T mix;
        ChorusType chorusType;

    public:
        Chorus(ChorusType chorusType, T sampleRate, T frequency, size_t baseDelaySamples, size_t depthSamples, T mix, LFO<T>::Waveform waveform = LFO<T>::Waveform::Sine)
            : baseDelaySamples(baseDelaySamples),
            depthSamples(depthSamples),
            mix(mix),
            chorusType(chorusType)
        {
            using Underlying = std::underlying_type_t<ChorusType>;
            auto voiceCount = static_cast<Underlying>(chorusType);

            if (voiceCount < static_cast<Underlying>(ChorusType::ChorusEngine) ||
                voiceCount > static_cast<Underlying>(ChorusType::TriChorus))
            {
                throw std::invalid_argument("Chorus type must be between ChorusEngine and TriChorus");
            }

            if (baseDelaySamples == 0)
            {
                throw std::invalid_argument("Base delay must be at least 1 sample");
            }

            if (mix < 0 || mix > 1)
            {
                throw std::invalid_argument("Mix must be between 0 and 1");
            }

            lfos.reserve(static_cast<size_t>(voiceCount));
            delays.reserve(static_cast<size_t>(voiceCount));

            size_t bufferSize = baseDelaySamples + depthSamples;

            for (size_t voice = 0; voice < static_cast<size_t>(voiceCount); ++voice)
            {
                T phase = static_cast<T>(voice) / static_cast<T>(voiceCount);
                lfos.emplace_back(sampleRate, frequency, waveform, phase);
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
