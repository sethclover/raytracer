#include <cmath>

#include "raytracer/quad.hpp"

quad::quad(const point3& Q, const vec3& u, const vec3& v, std::shared_ptr<material> mat)
: Q(Q), u(u), v(v), mat(mat) { 
    auto n = cross(u, v);
    normal = unit_vector(n);
    D = dot(normal, Q);
    w = n / dot(n, n);

    set_bounding_box(); 
}

void quad::set_bounding_box() {
    // Compute the bouding box of all four vertices
    auto bbox_diagonal1 = aabb(Q, Q + u + v);
    auto bbox_diagonal2 = aabb(Q + u, Q + v);
    bbox = aabb(bbox_diagonal1, bbox_diagonal2);
}

bool quad::hit_ab(double a, double b, hit_record& rec) const {
    interval unit_interval = interval(0, 1);
    // Given the hit point in plane cords, return false if it's outside the primitive
    // or set the hit record IV cords and return true

    if (!unit_interval.contains(a) || !unit_interval.contains(b)) return false;

    rec.u = a;
    rec.v = b;
    
    return true;
}

bool quad::hit(const ray& r, interval ray_t, hit_record& rec) const {
    auto denom = dot(normal, r.direction());

    // No hit if the ray is parallel to the plane
    if (std::fabs(denom) < 1e-8) {
        return false;
    }

    // Return false if the hit point parameter t is outside the ray interval
    auto t = (D - dot(normal, r.origin())) / denom;
    if (!ray_t.contains(t)) return false;

    // Determine of the hit point lies within the planar shape using its plane cords
    auto intersection = r.at(t);
    vec3 planar_hitpt_vec = intersection - Q;
    auto alpha = dot(w, cross(planar_hitpt_vec, v));
    auto beta = dot(w, cross(u, planar_hitpt_vec));

    if (!hit_ab(alpha, beta, rec)) return false;

    // Ray hits the 2D shape and set the rest of the hit record
    rec.t = t;
    rec.p = intersection;
    rec.mat = mat;
    rec.set_face_normal(r, normal);

    return true;
}
