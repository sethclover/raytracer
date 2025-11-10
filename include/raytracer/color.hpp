#pragma once

#include <iostream>

#include "raytracer/interval.hpp"
#include "raytracer/vec3.hpp"

using color = vec3;

namespace colors {
    static constexpr color black() { return color(0.0, 0.0, 0.0); }
    static constexpr color white() { return color(1.0, 1.0, 1.0); }
    static constexpr color red()   { return color(1.0, 0.0, 0.0); }
    static constexpr color green() { return color(0.0, 1.0, 0.0); }
    static constexpr color blue()  { return color(0.0, 0.0, 1.0); }
}

double linear_to_gamma(double linear_component);
void write_color(std::ostream& out, const color& pixel_color);
