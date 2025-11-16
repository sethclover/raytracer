#pragma once

#include <memory>

#include "raytracer/aabb.hpp"
#include "raytracer/color.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

class triangle : public hittable {
    public:
        triangle(const point3& v0, const point3& v1, const point3& v2, 
                 const color& c0, const color& c1, const color& c2,
                 std::shared_ptr<material> mat)
         : v0(v0), v1(v1), v2(v2), c0(c0), c1(c1), c2(c2), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
        color get_color(double u, double v) const;
        aabb bounding_box() const override { return bbox;}


    private: 
        point3 v0;
        point3 v1;
        point3 v2;
        color c0;
        color c1;
        color c2;
        std::shared_ptr<material> mat;
        aabb bbox;
};
