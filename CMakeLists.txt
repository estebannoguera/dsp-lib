cmake_minimum_required(VERSION 3.20)

project(
    DSPLib
    VERSION 0.1.0
    LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------
# Library
# ---------------------------

add_library(dsp INTERFACE)

target_include_directories(
    dsp
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/dsp
)

# ---------------------------
# Google Test
# ---------------------------

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

# ---------------------------
# Tests
# ---------------------------

add_executable(
    dsp_tests
    tests/containers/ring_buffer_tests.cpp
    tests/core/delay_line_tests.cpp
    tests/filters/moving_average_tests.cpp
    tests/filters/FIR_filter_tests.cpp
    tests/filters/biquad_tests.cpp
)

target_link_libraries(
    dsp_tests
    PRIVATE
        dsp
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(dsp_tests)