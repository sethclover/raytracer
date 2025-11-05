#pragma once

#include <iostream>

#include "raytracer/interval.hpp"
#include "raytracer/vec3.hpp"

using color = vec3;

double linear_to_gamma(double linear_component);
void write_color(std::ostream& out, const color& pixel_color);
