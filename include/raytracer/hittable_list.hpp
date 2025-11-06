#pragma once

#include <memory>
#include <vector>

#include "raytracer/hittable.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/ray.hpp"

class hittable_list : public hittable {
    public:
        std::vector<std::shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(std::shared_ptr<hittable> object);
        void clear() { objects.clear(); }
        void add(std::shared_ptr<hittable> object) { objects.push_back(object); }
        bool hit(const ray& r, interval& ray_t, hit_record& rec) const override;
        bool hitRecur(const ray& r, interval& ray_t, hit_record& rec) const;
};
