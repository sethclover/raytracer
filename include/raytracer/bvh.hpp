#pragma once

#include <memory>
#include <vector>

#include "raytracer/aabb.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/hittable_list.hpp"

class bvh_node : public hittable {
    public:
        bvh_node(hittable_list list) : bvh_node(list.objects, 0, list.objects.size()) {}
        bvh_node(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end);

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
        aabb bounding_box() const override { return bbox; }
    
    private:
        std::shared_ptr<hittable> left;
        std::shared_ptr<hittable> right;
        aabb bbox;

        static bool box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis_index) {
            auto a_axis_intvl = a->bounding_box().axis_interval(axis_index);
            auto b_axis_intvl = b->bounding_box().axis_interval(axis_index);

            return a_axis_intvl.min < b_axis_intvl.min;
        }
        static bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
            return box_compare(a, b, 0);
        }
        static bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
            return box_compare(a, b, 1);
        }
        static bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
            return box_compare(a, b, 2);
        }
};
