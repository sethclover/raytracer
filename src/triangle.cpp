#include <cmath>

#include "raytracer/triangle.hpp"
#include "raytracer/utils.hpp"

bool triangle::hit(const ray& r, interval ray_t, hit_record& rec) const {
    // Moller-Trumbore intersection algorithm
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;
    vec3 h = cross(r.direction(), edge2);
    double a = dot(edge1, h);
    if (std::fabs(a) < kEpsilon) return false; // Ray is parallel to triangle

    double f = 1.0 / a;
    vec3 s = r.origin() - v0;
    double u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) return false;

    vec3 q = cross(s, edge1);
    double v = f * dot(r.direction(), q);
    if (v < 0.0 || u + v > 1.0) return false;

    double t = f * dot(edge2, q);
    if (t < ray_t.min || t > ray_t.max) return false;

    rec.t = t;
    rec.p = r.at(t);
    vec3 outward_normal = cross(edge1, edge2);
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;
    rec.albedo = get_color(u, v);

    return true;
}

color triangle::get_color(double u, double v) const {
    double w = 1.0 - u - v;
    return w * c0 + u * c1 + v * c2;
}
