#include "containers/ring_buffer.hpp"

template<typename T>

class MovingAverageFilter{

    private:
        RingBuffer<float> history;
        size_t windowSize;
    
    public:
        explicit MovingAverageFilter(size_t windowSize)
            : history(windowSize),
            windowSize(windowSize)
        {
        }


        T processSample(T sample){

            history.write(sample);

            T sum = T{};

            for(size_t i = 0; i < windowSize; ++i)
            {
                sum += history.getDelayed(i);
            }

            return sum / static_cast<T>(windowSize);
        }
};