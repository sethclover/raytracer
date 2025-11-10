#pragma once

#include <cmath>
#include <memory>

#include "raytracer/color.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

class sphere : public hittable {
    public:
        sphere(const point3& center, double radius, color albedo, std::shared_ptr<material> mat)
         : center(center), radius(std::fmax(0, radius)), albedo(albedo), mat(mat) {}

        bool hit(const ray& r, interval& ray_t, hit_record& rec) const override;

    private: 
        point3 center;
        double radius;
        color albedo;
        std::shared_ptr<material> mat;
};
