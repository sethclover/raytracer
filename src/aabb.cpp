#include "raytracer/aabb.hpp"

const interval& aabb::axis_interval(int n) const {
    if (n == 1) return y;
    else if (n == 2) return z;
    else return x;
}

bool aabb::hit(const ray& r, interval ray_t) const {
    const point3& ray_orig = r.origin();
    const vec3& ray_dir = r.direction();

    for (int axis = 0; axis < 3; axis++) {
        const interval& axis_intvl = this->axis_interval(axis);
        const double adinv = 1.0 / ray_dir[axis];

        auto t0 = (axis_intvl.min - ray_orig[axis]) * adinv;
        auto t1 = (axis_intvl.max - ray_orig[axis]) * adinv;

        if (t0 < t1) {
            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;
        }
        else {
            if (t1 > ray_t.min) ray_t.min = t1;
            if (t0 < ray_t.max) ray_t.max = t0;
        }

        if (ray_t.max <= ray_t.min) return false;
    }

    return true;
}

int aabb::longest_axis() const {
    if (x.size() > y.size()) {
        return x.size() > z.size() ? 0 : 2;
    }
    else {
        return y.size() > z.size() ? 1 : 2;
    }
}
