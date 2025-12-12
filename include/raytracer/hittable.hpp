#pragma once

#include <memory>

#include "raytracer/aabb.hpp"
#include "raytracer/color.hpp"
#include "raytracer/interval.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

class material;

class hit_record {
    public:
        vec3 p;
        vec3 normal;
        std::shared_ptr<material> mat;
        double t;
        double u;
        double v;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal);
};

class hittable {
    public:
        virtual ~hittable() = default;
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
        virtual aabb bounding_box() const = 0;
};

class translate : public hittable {
    public:
        translate(std::shared_ptr<hittable> obj, const vec3& offset)
         : obj(obj), offset(offset) { bbox = obj->bounding_box() + offset; }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
        aabb bounding_box() const override { return bbox; }

    private:
        std::shared_ptr<hittable> obj;
        vec3 offset;
        aabb bbox;
};

class rotate_y : public hittable {
    public:
        rotate_y(std::shared_ptr<hittable> obj, double angle);

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
        aabb bounding_box() const override { return bbox; }

    private:
        std::shared_ptr<hittable> obj;
        double sin_theta;
        double cos_theta;
        aabb bbox;
};
