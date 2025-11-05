#pragma once

#include <cmath>
#include <memory>

#include "raytracer/hittable.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

class material;

class sphere : public hittable {
    public:
        sphere(const point3& center, double radius, std::shared_ptr<material> mat)
         : center(center), radius(std::fmax(0, radius)), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

    private: 
        point3 center;
        double radius;
        std::shared_ptr<material> mat;
};
