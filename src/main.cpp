#include <memory>

#include "raytracer/camera.hpp"
#include "raytracer/color.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/material.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/vec3.hpp"

int main(int argc, char** argv) {
    bool standard_scene = true;

    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "-s" || arg == "--simple") {
            standard_scene = false;
        }
        else if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [OPTION]\n"
                      << "Options:\n"
                      << "  -s, --simple    Render a simple scene\n"
                      << "  -h, --help      Show this help message\n";
            return 0;
        }
        else {
            std::cerr << "Unknown argument: " << arg << ". Use -h or --help for usage information.\n";
            return 1;
        }
    }
    else if (argc > 2) {
        std::cerr << "Too many arguments. Use -h or --help for usage information.\n";
        return 1;
    }

    hittable_list world;

    if (standard_scene) {
        /*
         * Standard scene
         */
        auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
        world.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

        for (int a = -11; a < 11; a++) {
            for (int b = -11; b < 11; b++) {
                auto choose_mat = random_double();
                point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

                if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                    std::shared_ptr<material> sphere_material;

                    if (choose_mat < 0.8) {
                        // diffuse
                        auto albedo = color::random() * color::random();
                        sphere_material = std::make_shared<lambertian>(albedo);
                        world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
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

        auto material1 = std::make_shared<dielectric>(1.5);
        world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

        auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
        world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

        auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
        world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    }
    else {
        /*
        * Simple scene
        */
        auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
        auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
        auto material_left   = std::make_shared<dielectric>(1.50);
        auto material_bubble = std::make_shared<dielectric>(1.00 / 1.50);
        auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

        world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
        world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
        world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
        world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
        world.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
    }

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 480;     // Increase image width for higher resolution
    cam.samples_per_pixel = 128; // Increase samples per pixel for better quality
    cam.max_depth = 16;          // Increase max depth for more ray bounces
    cam.vfov = 45;               // Set vertical field of view
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 10.0;    // Set defocus angle for depth of field effect
    cam.focus_dist = 3.4;        // Set focus distance

    cam.render(world);
}
