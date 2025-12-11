#pragma once

#include <memory>

#include "raytracer/aabb.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

class quad : public hittable {
    public:
        quad(const point3& Q, const vec3& u, const vec3& v, std::shared_ptr<material> mat);
        
        virtual void set_bounding_box();
        aabb bounding_box() const override { return bbox; }
        virtual bool hit_ab(double a, double b, hit_record& rec) const;
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

    private:
        point3 Q;
        vec3 u;
        vec3 v;
        vec3 w;
        std::shared_ptr<material> mat;
        aabb bbox;
        vec3 normal;
        double D;
};
