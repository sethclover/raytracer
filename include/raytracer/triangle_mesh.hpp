#pragma once

#include <memory>
#include <string>

#include "raytracer/aabb.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/triangle.hpp"

class triangle_mesh : public hittable {
    public:
        bool load_from_obj(const std::string& filepath, std::shared_ptr<material> material, double scale = 1.0);
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
        aabb bounding_box() const override { return bbox; }
    
    private:
        std::shared_ptr<hittable> bvh_root;
        aabb bbox;
};