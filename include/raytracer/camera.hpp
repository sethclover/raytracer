#pragma once

#include <iostream>
#include <string>

#include "raytracer/color.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

class camera {
    public:
        double aspect_ratio;    // Ratio of image width over height
        int image_width;        // Rendered image width in pixel count
        int samples_per_pixel;  // Count of random samples per pixel
        int max_depth;          // Maximum number of raybounces into scene
        color background;       // Scene background color
        double vfov;            // Vertical field of view in degrees
        point3 lookfrom;        // Point camera is looking from
        point3 lookat;          // Point camera is looking at
        vec3 vup;               // "Up" direction for camera orientation
        double defocus_angle;   // Defocus angle for depth of field effect
        double focus_dist;      // Distance from camera lookfrom to plan of focus

        camera() = default;

        void render(const hittable& world, const std::string& output_filename);

    private:
        int image_height;           // Rendered image height
        double pixel_samples_scale; // Color scale factor for a sum of pixel samples
        point3 center;              // Camera center
        point3 pixel00_loc;         // Location of pixel (0, 0)
        vec3 pixel_delta_u;         // Offset to pixel to the right
        vec3 pixel_delta_v;         // Offset to pixel below
        vec3 u, v, w;               // Camera coordinate system basis vectors
        vec3 defocus_disk_u;        // Defocus disk horizontal radius
        vec3 defocus_disk_v;        // Defocus disk vertical radius

        void initialize();
        ray get_ray(int i, int j) const;
        vec3 sample_square() const;
        point3 defocus_disk_sample() const;
        color ray_color(const ray& r, int depth, const hittable& world) const;
        color sample_pixel(int i, int j, const hittable& world) const;
};
