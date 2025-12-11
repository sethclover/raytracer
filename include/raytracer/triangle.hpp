#pragma once

#include <memory>

#include "raytracer/hittable.hpp"
#include "raytracer/material.hpp"
#include "raytracer/quad.hpp"
#include "raytracer/vec3.hpp"

class triangle : public quad {
    public:
        triangle(const point3& o, const vec3& aa, const vec3& ab, std::shared_ptr<material> mat)
         : quad(o, aa, ab, mat) {}

        virtual bool hit_ab(double a, double b, hit_record& rec) const;
};
