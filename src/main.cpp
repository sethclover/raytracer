#include <memory>
#include <iostream>

#include "bvh.hpp"
#include "raytracer/camera.hpp"
#include "raytracer/color.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/material.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/texture.hpp"
#include "raytracer/triangle.hpp"
#include "raytracer/vec3.hpp"

static constexpr unsigned SCENE_COUNT = 3;

// -h for help or number to choose from scenes
static const std::string_view usage = "Usage: raytracer [-h|--help|<scene_number>]\n"
                                          "  -h, --help       Show this help message\n"
                                          "  <scene_number>   Scene to render (1-" + std::to_string(SCENE_COUNT) + ")\n";

void bouncing_spheres() {
    hittable_list world;

    // Add ground with checker texture
    auto checker = std::make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(checker)));

    // Add random small spheres
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(std::make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    // Add three large spheres
    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // Build BVH for the world
    world = hittable_list(std::make_shared<bvh_node>(world));

    // Set up camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 16;
    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);
}

void checkered_spheres() {
    hittable_list world;

    // Add ground with checker texture
    auto checker = std::make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    // Add two large spheres
    world.add(std::make_shared<sphere>(point3(0, -10, 0), 10, std::make_shared<lambertian>(checker)));
    world.add(std::make_shared<sphere>(point3(0, 10, 0), 10, std::make_shared<lambertian>(checker)));
    
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 16;
    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world);
}

void earth() {
    auto earth_texture = std::make_shared<image_texture>("earthmap.png");
    auto earth_surface = std::make_shared<lambertian>(earth_texture);
    auto globe = std::make_shared<sphere>(point3(0, 0, 0), 2.0, earth_surface);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 16;
    cam.vfov = 20;
    cam.lookfrom = point3(0, 0, 12);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(hittable_list(globe));
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error: Expected 1 argument.\n" << usage;
        return 1;
    }

    std::string arg1 = argv[1];

    if (arg1 == "-h" || arg1 == "--help") {
        std::cout << usage;
        return 0;
    }

    unsigned scene_choice = 0;
    try {
        scene_choice = std::stoul(arg1);
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid argument '" << arg1 << "'.\n" << usage;
        return 1;
    }

    if (scene_choice == 0 || scene_choice > SCENE_COUNT) {
        std::cerr << "Error: Scene number out of range.\n" << usage;
        return 1;
    }

    if (scene_choice == 1) {
        bouncing_spheres();
    } else if (scene_choice == 2) {
        checkered_spheres();
    } else if (scene_choice == 3) {
        earth();
    }
}
