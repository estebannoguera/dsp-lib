#pragma once

#include <stdexcept>

template<typename T>

class RingBuffer {
    std::vector<T> buffer;
    size_t capacity;
    size_t writeIndex;
    size_t count;


    public:
    

    RingBuffer(size_t capacity)
        : buffer(capacity),
        capacity(capacity),
        writeIndex(0),
        count(0)
    {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
    }

    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;


    void write(T sample) {
        buffer[writeIndex] = sample;
        writeIndex = (writeIndex + 1) % capacity;
        if (count < capacity) {
            count++;
        }
    }

    T getDelayed(size_t samplesAgo) const {

        if(samplesAgo < 0)
        {
            return T{};
        }

        if(samplesAgo >= count)
        {
            return T{};
        }

        size_t latestIndex = (writeIndex - 1 + capacity) % capacity;

        size_t readIndex = (latestIndex - samplesAgo + capacity) % capacity;

        return buffer[readIndex];
    }

    int getCount() const {
        return count;
    }

    bool isFull() const {
        return count == capacity;
    }

    bool isEmpty() const {
        return count == 0;
    }
};