#include "raytracer/hittable_list.hpp"

void hittable_list::add(std::shared_ptr<hittable> object) {
    bbox = aabb(bbox, object->bounding_box());
    objects.push_back(std::move(object));
}

bool hittable_list::hit(const ray& r, interval ray_t, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (const auto& object : objects) {
        ray_t.max = closest_so_far;
        if (object->hit(r, ray_t, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
