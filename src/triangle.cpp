#include "raytracer/triangle.hpp"

bool triangle::hit_ab(double a, double b, hit_record& rec) const {
    if (a < 0 || b < 0 || (a + b) > 1) return false;

    rec.u = a;
    rec.v = b;

    return true;
}
