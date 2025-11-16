#pragma once

#include <cmath>

#include "raytracer/interval.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

class aabb {
    public:
        interval x, y, z;
        static const aabb empty;
        static const aabb infinite;

        aabb() = default;
        aabb(const interval& x, const interval& y, const interval& z)
         : x(x), y(y), z(z) {}
        aabb(const point3& a, const point3& b)
         : x(interval(std::fmin(a.x(), b.x()), std::fmax(a.x(), b.x()))),
           y(interval(std::fmin(a.y(), b.y()), std::fmax(a.y(), b.y()))),
           z(interval(std::fmin(a.z(), b.z()), std::fmax(a.z(), b.z()))) {}
        aabb(const aabb& box0, const aabb& box1)
         : x(interval(box0.x, box1.x)),
           y(interval(box0.y, box1.y)),
           z(interval(box0.z, box1.z)) {}

        const interval& axis_interval(int n) const;
        bool hit(const ray& r, interval ray_t) const;
        int longest_axis() const;
};

inline const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
inline const aabb aabb::infinite = aabb(interval::infinite, interval::infinite, interval::infinite);
