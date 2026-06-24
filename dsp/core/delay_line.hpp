#include "containers/ring_buffer.hpp"

template<typename T>

class DelayLine {

    private:
        RingBuffer<T> buffer;
        size_t delaySamples;

    public:
        explicit DelayLine(size_t delaySamples)
            : buffer(std::max(delaySamples, size_t(1))),
            delaySamples(delaySamples) {}

        T processSample(T inputSample) {
            
            if (delaySamples == 0)
            {
                return inputSample;
            }

            T delayedSample = buffer.getDelayed(delaySamples - 1);
            buffer.write(inputSample);
            return delayedSample;
        }

};