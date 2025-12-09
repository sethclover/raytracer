#pragma once

#include <cmath>

#include "raytracer/color.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/texture.hpp"
#include "raytracer/vec3.hpp"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& albedo) : tex(std::make_shared<solid_color>(albedo)) {}
        lambertian(std::shared_ptr<texture> texture) : tex(texture) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

    private:
        std::shared_ptr<texture> tex;
};

class metal : public material {
    public:
        metal(color albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

    private:
        color albedo;
        double fuzz;
};

class dielectric : public material {
    public:
        dielectric(double refraction_index) : refraction_index(refraction_index) {}
        
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
    
    private:
        // Refractive index in vacuum or air, or the ratio of the material's refractive index over
        // the refractive index of the enclosing media
        double refraction_index;
        static double reflectance(double cosine, double refraction_index);
};
