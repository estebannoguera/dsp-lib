#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>

class RingBuffer {

private:
  std::vector<T> buffer;
  size_t writeIndex;
  size_t count;

public:
  explicit RingBuffer(size_t capacity)
      : buffer(capacity), writeIndex(0), count(0) {
    if (capacity == 0) {
      throw std::invalid_argument("Capacity must be greater than 0");
    }
  }

  RingBuffer(const RingBuffer &) = delete;
  RingBuffer &operator=(const RingBuffer &) = delete;

  RingBuffer(RingBuffer &&other) noexcept
      : buffer(std::move(other.buffer)),
        writeIndex(std::exchange(other.writeIndex, 0)),
        count(std::exchange(other.count, 0)) {}

  RingBuffer &operator=(RingBuffer &&other) noexcept {
    if (this != &other) {
      buffer = std::move(other.buffer);
      writeIndex = std::exchange(other.writeIndex, 0);
      count = std::exchange(other.count, 0);
    }
    return *this;
  }

  void write(T sample) {
    buffer[writeIndex] = sample;
    writeIndex = (writeIndex + 1) % buffer.size();
    if (count < buffer.size()) {
      count++;
    }
  }

  T getDelayed(size_t samplesAgo) const {

    if (samplesAgo >= count) {
      return T{};
    }

    size_t readIndex =
        (writeIndex - 1 - samplesAgo + buffer.size()) % buffer.size();
    return buffer[readIndex];
  }

  size_t getCount() const { return count; }

  bool isFull() const { return count == buffer.size(); }

  bool isEmpty() const { return count == 0; }
};