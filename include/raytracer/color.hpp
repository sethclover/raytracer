#pragma once

#include <cmath>
#include <iostream>

#include "raytracer/interval.hpp"
#include "raytracer/vec3.hpp"

using color = vec3;

namespace colors {
    inline color black()   { return color(0.0, 0.0, 0.0); }
    inline color white()   { return color(1.0, 1.0, 1.0); }
    inline color red()     { return color(1.0, 0.0, 0.0); }
    inline color green()   { return color(0.0, 1.0, 0.0); }
    inline color blue()    { return color(0.0, 0.0, 1.0); }
    inline color yellow()  { return color(1.0, 1.0, 0.0); }
    inline color magenta() { return color(1.0, 0.0, 1.0); }
    inline color cyan()    { return color(0.0, 1.0, 1.0); }
    inline color orange()  { return color(1.0, 0.5, 0.0); }
    inline color purple()  { return color(0.5, 0.0, 0.5); }
}

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) return std::sqrt(linear_component);
    else return 0.0;
}
