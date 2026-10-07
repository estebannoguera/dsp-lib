#include <cmath>
#include <limits>

template <typename T>
T amplitudeToDecibels(T amplitude)
{
    if (amplitude == T(0))
    {
        return -std::numeric_limits<T>::infinity();
    }

    return T(20) * std::log10(amplitude);
}

template <typename T>
T compressLevel(T levelDb, T thresholdDb, T ratio)
{
    if (levelDb <= thresholdDb)
    {
        return levelDb;
    }

    return thresholdDb + (levelDb - thresholdDb) / ratio;
}

template <typename T>
T decibelsToLinear(T decibels)
{
    return std::pow(T(10), decibels / T(20));
}