#include <algorithm>

#include "raytracer/bvh.hpp"

bvh_node::bvh_node(std::vector<std::shared_ptr<hittable>>& objects, size_t start, size_t end) {
    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare : (axis == 1 ? box_y_compare : box_z_compare);
    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    }
    else if (object_span == 2) {
        left = objects[start];
        right = objects[start + 1];
    }
    else {
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

        auto mid = start + object_span / 2;
        left = std::make_shared<bvh_node>(objects, start, mid);
        right = std::make_shared<bvh_node>(objects, mid, end);
    }

    bbox = aabb(left->bounding_box(), right->bounding_box());
}

bool bvh_node::hit(const ray& r, interval ray_t, hit_record& rec) const {
    if (!bbox.hit(r, ray_t)) return false;

    bool hit_left = left->hit(r, ray_t, rec);
    bool hit_right = right->hit(r, interval(ray_t.min, (hit_left ? rec.t : ray_t.max)), rec);

    return hit_left || hit_right;
}
