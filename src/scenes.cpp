#include <memory>
#include <iostream>

#include "bvh.hpp"
#include "raytracer/camera.hpp"
#include "raytracer/color.hpp"
#include "raytracer/constant_medium.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/material.hpp"
#include "raytracer/quad.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/texture.hpp"
#include "raytracer/triangle.hpp"
#include "raytracer/triangle_mesh.hpp"
#include "raytracer/vec3.hpp"

void bouncing_spheres_1(unsigned num_threads) {
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
    cam.background = color(0.7, 0.8, 1.0);
    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world, "images/bouncing_spheres_1.ppm", num_threads);
}

void checkered_spheres_2(unsigned num_threads) {
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
    cam.background = color(0.7, 0.8, 1.0);
    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world, "images/checkered_spheres_2.ppm", num_threads);
}

void earth_3(unsigned num_threads) {
    hittable_list world;

    auto earth_texture = std::make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = std::make_shared<lambertian>(earth_texture);
    auto globe = std::make_shared<sphere>(point3(0, 0, 0), 2.0, earth_surface);

    auto sun_surface = std::make_shared<diffuse_light>(color(4, 4, 4));
    auto sun = std::make_shared<sphere>(point3(20, 10, 20), 7.0, sun_surface);

    world.add(globe);
    world.add(sun);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 200;
    cam.max_depth = 32;
    cam.background = colors::black();
    cam.vfov = 25;
    cam.lookfrom = point3(0, 0, 12);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(hittable_list(world), "images/earth_3.ppm", num_threads);
}

void perlin_spheres_4(unsigned num_threads) {
    hittable_list world;

    auto pertext = std::make_shared<noise_texture>(4);
    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(pertext)));
    world.add(std::make_shared<sphere>(point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 16;
    cam.background = color(0.7, 0.8, 1.0);
    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world, "images/perlin_spheres_4.ppm", num_threads);
}

void quads_5(unsigned num_threads) {
    hittable_list world;

    // Materials
    auto left_red     = std::make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = std::make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = std::make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = std::make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(std::make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(std::make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(std::make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(std::make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(std::make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 16;
    cam.background = color(0.7, 0.8, 1.0);
    cam.vfov = 80;
    cam.lookfrom = point3(0, 0, 9);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world, "images/quads_5.ppm", num_threads);
}

void octohedron_6(unsigned num_threads) {
    hittable_list world;

    // Materials
    auto red     = std::make_shared<lambertian>(colors::red());
    auto green   = std::make_shared<lambertian>(colors::green());
    auto blue    = std::make_shared<lambertian>(colors::blue());
    auto yellow  = std::make_shared<lambertian>(colors::yellow());
    auto magenta = std::make_shared<lambertian>(colors::magenta());
    auto cyan    = std::make_shared<lambertian>(colors::cyan());
    auto orange  = std::make_shared<lambertian>(colors::orange());
    auto purple  = std::make_shared<lambertian>(colors::purple());
    
    point3 top(0, 2, 0);
    point3 bottom(0, -2, 0);

    // Octahedron top pyramid
    world.add(std::make_shared<triangle>(top, vec3(-2,-2, 2), vec3( 2,-2, 2), red));    // front
    world.add(std::make_shared<triangle>(top, vec3(-2,-2,-2), vec3(-2,-2, 2), green));  // left
    world.add(std::make_shared<triangle>(top, vec3( 2,-2, 2), vec3( 2,-2,-2), blue));   // right
    world.add(std::make_shared<triangle>(top, vec3( 2,-2,-2), vec3(-2,-2,-2), yellow)); // back
    

    // Octahedron bottom pyramid
    world.add(std::make_shared<triangle>(bottom, vec3( 2, 2, 2), vec3(-2, 2, 2), magenta)); // front
    world.add(std::make_shared<triangle>(bottom, vec3(-2, 2, 2), vec3(-2, 2,-2), cyan));    // left
    world.add(std::make_shared<triangle>(bottom, vec3(-2, 2,-2), vec3( 2, 2,-2), orange));  // back
    world.add(std::make_shared<triangle>(bottom, vec3( 2, 2,-2), vec3( 2, 2, 2), purple));  // right

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 100;
    cam.max_depth = 16;
    cam.background = color(0.7, 0.8, 1.0);
    cam.vfov = 40;
    cam.lookfrom = point3(5, 1, 8);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world, "images/octohedron_6.ppm", num_threads);
}

void simple_light_7(unsigned num_threads) {
    hittable_list world;

    auto pertext = std::make_shared<noise_texture>(4);
    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(pertext)));
    world.add(std::make_shared<sphere>(point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

    auto difflight = std::make_shared<diffuse_light>(color(4, 4, 4));
    world.add(std::make_shared<sphere>(point3(0, 7, 0), 2, difflight));
    world.add(std::make_shared<quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 16;
    cam.background = colors::black();
    cam.vfov = 20;
    cam.lookfrom = point3(26, 3, 6);
    cam.lookat = point3(0, 2, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world, "images/simple_light_7.ppm", num_threads);
}

void cornell_box_8(unsigned num_threads) {
    hittable_list world;

    auto red = std::make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = std::make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = std::make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

    world.add(std::make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(std::make_shared<quad>(point3(0, 0, 0), vec3(0, 0, 555), vec3(0, 555, 0), red));
    world.add(std::make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
    world.add(std::make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(std::make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
    world.add(std::make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

    std::shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    std::shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, vec3(130, 0, 65));
    world.add(box2);

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 200;
    cam.max_depth = 32;
    cam.background = colors::black();
    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world, "images/cornell_box_8.ppm", num_threads);
}

void cornell_smoke_9(unsigned num_threads) {
    hittable_list world;

    auto red = std::make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = std::make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = std::make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = std::make_shared<diffuse_light>(color(7, 7, 7));

    world.add(std::make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(std::make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(std::make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
    world.add(std::make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(std::make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(std::make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

    std::shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, vec3(265, 0, 295));

    std::shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, vec3(130, 0, 65));

    world.add(std::make_shared<constant_medium>(box1, 0.01, color(0.0, 0.0, 0.0)));
    world.add(std::make_shared<constant_medium>(box2, 0.01, color(1.0, 1.0, 1.0)));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 200;
    cam.max_depth = 32;
    cam.background = colors::black();
    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world, "images/cornell_smoke_9.ppm", num_threads);
}

void next_week_10(unsigned num_threads) {
    hittable_list boxes1;
    auto ground = std::make_shared<lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            double w = 100;
            double x0 = -1000 + i * w;
            double z0 = -1000 + j * w;
            double y0 = 0.0;
            double x1 = x0 + w;
            double y1 = random_double(1, 101);
            double z1 = z0 + w;

            boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list world;

    world.add(std::make_shared<bvh_node>(boxes1));

    auto light = std::make_shared<diffuse_light>(color(7, 7, 7));
    world.add(std::make_shared<quad>(point3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto sphere_material = std::make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(std::make_shared<sphere>(center1, center2, 50, sphere_material));

    world.add(std::make_shared<sphere>(point3(260, 150, 45), 50, std::make_shared<dielectric>(1.5)));
    world.add(std::make_shared<sphere>(point3(0, 150, 145), 50, std::make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = std::make_shared<sphere>(point3(360, 150, 145), 70, std::make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(std::make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<sphere>(point3(0, 0, 0), 5000, std::make_shared<dielectric>(1.5));
    world.add(std::make_shared<constant_medium>(boundary, 0.0001, color(1, 1, 1)));

    auto emat = std::make_shared<lambertian>(std::make_shared<image_texture>("earthmap.jpg"));
    world.add(std::make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = std::make_shared<noise_texture>(0.2);
    world.add(std::make_shared<sphere>(point3(220, 280, 300), 80, std::make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = std::make_shared<lambertian>(color(0.73, 0.73, 0.73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<sphere>(point3::random(0,165), 10, white));
    }

    world.add(std::make_shared<translate>(std::make_shared<rotate_y>(std::make_shared<bvh_node>(boxes2), 15), vec3(-100, 270, 395)));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 2000;
    cam.max_depth = 40;
    cam.background = colors::black();
    cam.vfov = 40;
    cam.lookfrom = point3(478, 278, -600);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world, "images/next_week_10.ppm", num_threads);
}

void triangle_mesh_11(unsigned num_threads) {
    hittable_list world;

    auto material = std::make_shared<lambertian>(colors::red());
    auto mesh = std::make_shared<triangle_mesh>();
    if (!mesh->load_from_obj("models/teapot.obj", material)) {
        std::cerr << "Failed to load triangle mesh." << std::endl;
        return;
    }
    world.add(mesh);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 16;
    cam.background = color(0.7, 0.8, 1.0);
    cam.vfov = 50;
    cam.lookfrom = point3(0, 2, 5);
    cam.lookat = point3(0, 1, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world, "images/triangle_mesh_11.ppm", num_threads);
}

void final_render_12(unsigned num_threads) {
    hittable_list boxes1;
    auto ground = std::make_shared<lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            double w = 100;
            double x0 = -1000 + i * w;
            double z0 = -1000 + j * w;
            double y0 = 0.0;
            double x1 = x0 + w;
            double y1 = random_double(1, 91);
            double z1 = z0 + w;

            // Lavender purple for lower boxes
            auto box_material = std::make_shared<lambertian>(color(0.78, 0.60, 0.96));
            boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), box_material));
        }
    }

    hittable_list world;

    world.add(std::make_shared<bvh_node>(boxes1));

    // Light source
    auto light = std::make_shared<diffuse_light>(color(7, 7, 7));
    world.add(std::make_shared<quad>(point3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light));

    // Original spheres from next_week_10
    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto sphere_material = std::make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(std::make_shared<sphere>(center1, center2, 50, sphere_material));

    // Glass ball (dielectric)
    world.add(std::make_shared<sphere>(point3(260, 150, 45), 50, std::make_shared<dielectric>(1.5)));
    
    // Metal sphere
    world.add(std::make_shared<sphere>(point3(0, 150, 145), 50, std::make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = std::make_shared<sphere>(point3(360, 150, 145), 70, std::make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(std::make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<sphere>(point3(0, 0, 0), 5000, std::make_shared<dielectric>(1.5));
    world.add(std::make_shared<constant_medium>(boundary, 0.0001, color(1, 1, 1)));

    // Earth sphere
    auto emat = std::make_shared<lambertian>(std::make_shared<image_texture>("earthmap.jpg"));
    world.add(std::make_shared<sphere>(point3(400, 200, 400), 100, emat));
    
    auto pertext = std::make_shared<noise_texture>(1);
    world.add(std::make_shared<sphere>(point3(220, 280, 300), 80, std::make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = std::make_shared<lambertian>(color(0.73, 0.73, 0.73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    world.add(std::make_shared<translate>(std::make_shared<rotate_y>(std::make_shared<bvh_node>(boxes2), 15), vec3(-100, 270, 395)));

    // Add teapot mesh (red) - positioned below glass ball at y=50
    auto teapot_material = std::make_shared<lambertian>(colors::red());
    auto teapot = std::make_shared<triangle_mesh>();
    if (!teapot->load_from_obj("models/teapot.obj", teapot_material, 13.0)) {
        std::cerr << "Failed to load teapot mesh." << std::endl;
        return;
    }
    world.add(std::make_shared<translate>(std::make_shared<rotate_y>(teapot, 15), vec3(260, 250, 45)));

    // Add Homer mesh (yellow) - positioned on top of earth
    auto homer_material = std::make_shared<lambertian>(color(1.0, 1.0, 0.0));  // yellow
    auto homer = std::make_shared<triangle_mesh>();
    if (!homer->load_from_obj("models/homer.obj", homer_material, 210.0)) {
        std::cerr << "Failed to load Homer mesh." << std::endl;
        return;
    }
    world.add(std::make_shared<translate>(std::make_shared<rotate_y>(homer, -155), vec3(380, 320, 360)));

    // Camera setup
    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 350;
    cam.samples_per_pixel = 350;
    cam.max_depth = 20;
    cam.background = colors::black();
    cam.vfov = 40;
    cam.lookfrom = point3(478, 278, -600);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world, "images/final_render_12.ppm", num_threads);
}
