# DSP-LIB

A header-only DSP library in C++20. Each block processes one sample per call through `processSample` or, for the LFO, through `tick`. CMake adds the `dsp/` directory to the include path, so a client includes `"containers/ring_buffer.hpp"` or `"modulation/lfo.hpp"`.

The library target is `INTERFACE`: it does not build its own binary. The code is compiled into the client when the header is included. The language standard is C++20.

## Build and test

CMake 3.20 or newer and a C++20 compiler are required. GoogleTest 1.17 is downloaded during configuration.

```bash
./run_tests.sh
```

That configures `build/`, builds `dsp_tests`, and runs `ctest`. The script uses `sysctl` for the core count, so it targets macOS. Elsewhere:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Layout

```text
dsp/
  containers/     RingBuffer
  core/           DelayLine, constants, biquad coefficients
  filters/        moving average, FIR, biquad, RBJ design
  modulation/     LFO, Chorus
  generators/     reserved; generate_white_noise.hpp is empty
tests/            one file per block, matching the source folders
docs/ROADMAP.md   phased plan
```

Most classes are `template<typename T>`. The tests use `float`.

## RingBuffer

`dsp/containers/ring_buffer.hpp`

Circular history. The constructor takes the capacity and throws `std::invalid_argument` when it is 0. Copy and assignment are deleted, and there is no move constructor, so a `RingBuffer` cannot be stored by value in a `std::vector`. `Chorus` keeps one behind a `std::unique_ptr` for that reason.

- `write(sample)` advances the index and, until the buffer is full, increments the count.
- `getDelayed(samplesAgo)` returns the sample written `samplesAgo` steps back. `0` is the most recently written sample. If that index does not exist yet, it returns `T{}`.
- `getCount`, `isFull`, and `isEmpty` report how much of the buffer is occupied.

`samplesAgo` is a `size_t`, so the `samplesAgo < 0` branch never runs.

Callers that read before they write, such as `DelayLine` and `Chorus`, request `getDelayed(delaySamples - 1)` for a delay of `delaySamples`. In that order, `getDelayed(0)` is the previous sample.

## DelayLine

`dsp/core/delay_line.hpp`

```cpp
DelayLine<float> delay(bufferSize, delaySamples, feedback, mix);
```

`feedback` and `mix` must lie in `[0, 1]`. Outside that range the constructor throws `std::invalid_argument`.

`processSample` reads the delay, mixes, and then writes. When `delaySamples == 0` it returns the input and does not write. Otherwise:

```text
output = delayed * mix + input * (1 - mix)
write(input + delayed * feedback)
```

`mix == 0` is the dry signal. `mix == 1` is the delayed signal alone. `feedback` recirculates into the buffer. `feedback` and `mix` are `float` even when `T` is another type. A `bufferSize` of 0 is raised to 1 before the `RingBuffer` is constructed.

## FIR filters

### MovingAverageFilter

`dsp/filters/moving_average_filter.hpp`

```cpp
explicit MovingAverageFilter(size_t windowSize);
```

It writes the sample and returns the average of the last `windowSize` positions. Positions that do not exist yet are 0, so the average starts attenuated until the window fills. `windowSize == 0` fails inside the `RingBuffer`. The internal history is a `RingBuffer<float>`, not a `RingBuffer<T>`.

### FIRFilter

`dsp/filters/fir_filter.hpp`

```cpp
explicit FIRFilter(const std::vector<T>& coefficients);
```

An empty coefficient vector throws `std::invalid_argument`. `coefficients[0]` multiplies the current sample, `coefficients[1]` the previous one, and so on. This is direct convolution against the `RingBuffer`.

## Biquad

Coefficients live in `BiquadCoefficients<T>`: `b0`, `b1`, `b2`, `a1`, `a2`. `a0` is not stored; the designers already divide every coefficient by `a0`.

The `BiquadEngine` equation is direct form I:

```text
y = b0*x + b1*x1 + b2*x2 - a1*y1 - a2*y2
```

`a1` and `a2` use the sign from Robert Bristow-Johnson's cookbook. Until `setCoefficients` is called, the coefficients are 0 and the output is silence. `reset()` clears the state `x1`, `x2`, `y1`, `y2`.

`BiquadDF2T` is transposed direct form II:

```text
y  = b0*x + s1
s1 = b1*x - a1*y + s2
s2 = b2*x - a2*y
```

`processSample` is fixed to `float`, not `T`. `reset()` clears `x1`, `x2`, `y1`, and `y2`, and leaves `s1` and `s2` unchanged.

### LPF and HPF

`dsp/filters/rbj_design.hpp`

```cpp
LPF<float>::calculate(sampleRate, f0, Q);
HPF<float>::calculate(sampleRate, f0, Q);
```

Both return coefficients that are already normalized. `omega = 2π * f0 / sampleRate` and `alpha = sin(omega) / (2Q)`. `PI` and `TWO_PI` in `dsp/core/constants.hpp` are `float`.

## LFO

`dsp/modulation/lfo.hpp`

```cpp
explicit LFO(T sampleRate,
             T frequency = 1,
             Waveform waveform = Waveform::Sine,
             T phase = 0);
```

`explicit` matters because the remaining parameters have defaults, so the constructor can be called with a single argument.

| Argument | Rule |
| --- | --- |
| `sampleRate` | Must be greater than 0 |
| `frequency` | Hertz. `0` holds the phase still. A negative value throws |
| `phase` | Fraction of a cycle in `[0, 1)`, not radians |

`tick()` returns the waveform and then adds `frequency / sampleRate`. If the phase reaches 1 or more, it subtracts 1 once. An LFO advances by less than one cycle per sample, so one subtraction is enough. Wrapping keeps a `float` from losing the increment once the phase grows without bound.

Waveforms, with phase in `[0, 1)`:

| Waveform | Output |
| --- | --- |
| `Triangle` | Rises from 0 to 1 over the first half and falls from 1 to 0 over the second |
| `Saw` | The phase itself, from 0 toward 1 |
| `ReverseSaw` | `1 - phase` |
| `Square` | 1 on `[0, 0.5)` and 0 on `[0.5, 1)` |
| `Sine` | `std::sin(phase)` with phase in cycles, so the argument is not `phase * 2π`. It covers the first radian and, when the cycle wraps, jumps from `sin(1)` back to `0` |

`prepare`, `reset`, `setFrequency`, `setWaveform`, and `processSample` are declared and have no definition. Calling them fails to link.

## Chorus

`dsp/modulation/chorus.hpp`

```cpp
Chorus<float> chorus(Chorus<float>::ChorusType::TriChorus,
                     sampleRate, frequency,
                     baseDelaySamples, depthSamples, mix);
```

`ChorusType` is the voice count:

| Value | Voices | Phases |
| --- | --- | --- |
| `ChorusEngine = 1` | 1 | `0` |
| `DualChorus = 2` | 2 | `0`, `0.5` |
| `TriChorus = 3` | 3 | `0`, `1/3`, `2/3` |

Those fractions are the same positions as `0`, `2π/3`, and `4π/3` in radians. Each voice has its own triangle LFO and its own buffer. The buffer holds `baseDelaySamples + depthSamples` samples.

`baseDelaySamples` must be at least 1. `mix` must lie in `[0, 1]`. A `ChorusType` outside 1..3, produced with `static_cast`, throws.

On each sample a voice's delay is:

```text
samples = baseDelaySamples + (LFO value * depthSamples)
```

The triangle runs from 0 to 1, so the depth is an integer number of extra samples from 0 through `depthSamples`. The buffer is read, then the input is written.

`processSample` returns the mono mix:

```text
wet = average of the voices
output = input * (1 - mix) + wet * mix
```

If an array is passed, each element receives that voice's delayed sample before the average and before `mix`. That array is what gets panned outside the class. The return value has already summed the voices, so panning it sends the same signal to both channels.

```cpp
float voices[3];
float mono = chorus.processSample(input, voices);

float left = voices[0];
float center = voices[1];
float right = voices[2];
```

With no pointer, or with `nullptr`, only the mono mix is used.

## Tests

| File | What it locks down |
| --- | --- |
| `tests/containers/ring_buffer_tests.cpp` | Capacity, writes, delay, wrap |
| `tests/core/delay_line_tests.cpp` | Mix, feedback, ranges |
| `tests/filters/moving_average_tests.cpp` | Average |
| `tests/filters/FIR_filter_tests.cpp` | Empty coefficients and convolution |
| `tests/filters/biquad_tests.cpp` | Direct form I and an RBJ low-pass |
| `tests/filters/biquad_DF2T_tests.cpp` | Transposed form |
| `tests/modulation/lfo_tests.cpp` | Waveshapes, phase wrap, initial phase |
| `tests/modulation/chorus_tests.cpp` | One, two, and three voices, base delay, voices kept separate for panning |

What is still planned is in [docs/ROADMAP.md](docs/ROADMAP.md).
