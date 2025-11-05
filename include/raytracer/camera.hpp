#pragma once

#include <iostream>

#include "raytracer/color.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

class camera {
    public:
        double aspect_ratio = 1.0;  // Ratio of image width over height
        int image_width = 100;      // Rendered image width in pixel count
        int samples_per_pixel = 10; // Count of random samples per pixel
        int max_depth = 10;         // Maximum number of raybounces into scene

        void render(const hittable& world);

    private:
        int image_height;           // Rendered image height
        double pixel_samples_scale; // Color scale factor for a sum of pixel samples
        point3 center;              // Camera center
        point3 pixel00_loc;         // Location of pixel (0, 0)
        vec3 pixel_delta_u;         // Offset to pixel to the right
        vec3 pixel_delta_v;         // Offset to pixel below

        void initialize();
        ray get_ray(int i, int j) const;
        vec3 sample_square() const;
        color ray_color(const ray& r, int depth, const hittable& world) const;
};
