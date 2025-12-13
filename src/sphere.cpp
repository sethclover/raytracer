#include <numbers>

#include "raytracer/sphere.hpp"

bool sphere::hit(const ray& r, interval ray_t, hit_record& rec) const {
    point3 curr_center = center.at(r.time());
    vec3 oc = curr_center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0) return false;

    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - curr_center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat = mat;

    return true;
}

void sphere::get_sphere_uv(const point3& p, double& u, double& v) const {
    // p: a given point on the sphere of radius one, centered at the origin
    // u: returned value [0,1] of angle around the Y axis from X=-1
    // v: returned value [0,1] of angle from Y=-1 to Y=+1

    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + std::numbers::pi;

    u = phi / (2 * std::numbers::pi);
    v = theta / std::numbers::pi;
}
