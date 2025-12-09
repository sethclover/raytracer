#pragma once

#include <memory>

#include "raytracer/color.hpp"
#include "raytracer/rtw_std_image.hpp"
#include "raytracer/vec3.hpp"

class texture {
    public:
        virtual ~texture() = default;

        virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color() = default;
        solid_color(const color& albedo) : albedo(albedo) {}

        color value( double u [[maybe_unused]], double v [[maybe_unused]], const point3& p [[maybe_unused]]) const override {
             return albedo;
        }

    private:
        color albedo;
};

class checker_texture : public texture {
    public:
        checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd) 
         : inv_scale(1.0 / scale), even(even), odd(odd) {}
        checker_texture(double scale, const color& c1, const color& c2)
         : checker_texture(scale, std::make_shared<solid_color>(c1), std::make_shared<solid_color>(c2)) {}

        color value(double u, double v, const point3& p) const override;

    private:
        double inv_scale;
        std::shared_ptr<texture> even;
        std::shared_ptr<texture> odd;
};

class image_texture : public texture {
    public:
        image_texture(const char* filename) : image(filename) {}

        color value(double u, double v, const point3& p [[maybe_unused]]) const override;

    private:
        rtw_image image;
};
