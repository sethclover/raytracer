#pragma once

#include <cmath>
#include <memory>

#include "raytracer/aabb.hpp"
#include "raytracer/color.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

class sphere : public hittable {
    public:
        // Stationary sphere
        sphere(const point3& static_center, double radius, color albedo, std::shared_ptr<material> mat)
         : center(static_center, vec3(0, 0, 0)), radius(std::fmax(0, radius)), albedo(albedo), mat(mat) {
            auto rvec = vec3(radius, radius, radius);
            bbox = aabb(static_center - rvec, static_center + rvec);
         }
        // Moving sphere
        sphere(const point3& center1, const point3& center2, double radius, color albedo, std::shared_ptr<material> mat)
         : center(center1, center2 - center1), radius(std::fmax(0, radius)), albedo(albedo), mat(mat) {
            auto rvec = vec3(radius, radius, radius);
            aabb box1(center.at(0) - rvec, center.at(0) + rvec);
            aabb box2(center.at(1) - rvec, center.at(1) + rvec);
            bbox = aabb(box1, box2);
         }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
        aabb bounding_box() const override { return bbox;}

    private: 
        ray center;
        double radius;
        color albedo;
        std::shared_ptr<material> mat;
        aabb bbox;
};
