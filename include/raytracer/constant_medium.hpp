#pragma once

#include <memory>

#include "raytracer/aabb.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/texture.hpp"

class constant_medium : public hittable {
    public:
        constant_medium(std::shared_ptr<hittable> boundary, double density, std::shared_ptr<texture> tex)
         : boundary(boundary), neg_inv_density(-1 / density), phase_function(std::make_shared<isotropic>(tex)) {}
        constant_medium(std::shared_ptr<hittable> boundary, double density, const color& albedo)
         : boundary(boundary), neg_inv_density(-1 / density), phase_function(std::make_shared<isotropic>(albedo)) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
        aabb bounding_box() const override { return boundary->bounding_box(); }

    private:
        std::shared_ptr<hittable> boundary;
        double neg_inv_density;
        std::shared_ptr<material> phase_function;
};
