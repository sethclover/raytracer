#pragma once

#include <iostream>

#include "vec3.hpp"

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to [0,255]
    int rByte = static_cast<int>(255.999 * r);
    int gByte = static_cast<int>(255.999 * g);
    int bByte = static_cast<int>(255.999 * b);

    // Write out the pixel color components
    out << rByte << ' ' << gByte << ' ' << bByte << std::endl;
}
