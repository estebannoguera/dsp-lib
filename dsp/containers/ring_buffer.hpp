#pragma once

#include <stdexcept>

template<typename T>

class RingBuffer {
    int capacity;
    T* buffer;
    int writeIndex;
    int count;

    public:
    

    RingBuffer(int capacity) : capacity(capacity), writeIndex(0), count(0) {
        if (capacity <= 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
        buffer = new T[capacity];
    }

    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;

    ~RingBuffer() {
    delete[] buffer;
}

    void write(T sample) {
        buffer[writeIndex] = sample;
        writeIndex = (writeIndex + 1) % capacity;
        if (count < capacity) {
            count++;
        }
    }

    T getDelayed(int samplesAgo) const {

        if(samplesAgo < 0)
        {
            return T{};
        }

        if(samplesAgo >= count)
        {
            return T{};
        }
        
        int latestIndex = (writeIndex - 1 + capacity) % capacity;

        int readIndex = (latestIndex - samplesAgo + capacity) % capacity;
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