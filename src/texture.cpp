#include "raytracer/texture.hpp"

color checker_texture::value(double u, double v, const point3& p) const {
    auto x = static_cast<int>(std::floor(inv_scale * p.x()));
    auto y = static_cast<int>(std::floor(inv_scale * p.y()));
    auto z = static_cast<int>(std::floor(inv_scale * p.z()));

    bool isEven = (x + y + z) % 2 == 0;

    return isEven ? even->value(u, v, p) : odd->value(u, v, p); 
}

color image_texture::value(double u, double v, const point3& p [[maybe_unused]]) const {
    // If we have no texture data, then return solid cyan as a debugging aid
    if (image.height() <= 0) return color(0, 1, 1);

    // Clamp input texture coordinates to [0, 1] x [1, 0]
    u = interval(0, 1).clamp(u);
    v = 1.0 - interval(0, 1).clamp(v); // Flip V to image coordinates

    auto i = static_cast<int>(u * image.width());
    auto j = static_cast<int>(v * image.height());
    auto pixel = image.pixel_data(i, j);

    auto color_scale = 1.0 / 255.0;
    return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}

color noise_texture::value(double u [[maybe_unused]], double v [[maybe_unused]], const point3& p) const {
    return color(0.5, 0.5, 0.5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
}
