#pragma once

#include <limits>
#include <random>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;
constexpr float kEpsilon = 1e-8;

constexpr double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator(std::random_device{}());
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
    // Return a random interger in [min,max]
    return static_cast<int>(random_double(min, max + 1));
}
