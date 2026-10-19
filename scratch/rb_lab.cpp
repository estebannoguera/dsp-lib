#include "containers/ring_buffer.hpp"
#include <cstdio>
#include <type_traits>

int main(){

    std::printf("move constuctible: %d\n", std::is_move_constructible_v<RingBuffer<float>>);

    RingBuffer<float> rb(3);
    for (float x : {1.f, 2.f, 3.f, 4.f, 5.f}) rb.write(x);
    for (size_t k = 0; k < rb.getCount(); ++k)
        std::printf("getDelayed(%zu) = %.0f\n", k, rb.getDelayed(k));
}
