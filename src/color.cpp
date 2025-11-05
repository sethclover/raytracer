#include <cmath>

#include "raytracer/color.hpp"

double linear_to_gamma(double linear_component) {
    if (linear_component > 0) return std::sqrt(linear_component);
    else return 0.0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to [0,255]
    static const interval intensity(0.0, 0.999);
    int rByte = static_cast<int>(256 * intensity.clamp(r));
    int gByte = static_cast<int>(256 * intensity.clamp(g));
    int bByte = static_cast<int>(256 * intensity.clamp(b));

    // Write out the pixel color components
    out << rByte << ' ' << gByte << ' ' << bByte << std::endl;
}
