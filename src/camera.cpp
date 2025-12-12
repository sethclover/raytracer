#include <atomic>
#include <chrono>
#include <thread>

#include "raytracer/camera.hpp"

namespace {
    std::atomic<int> lines_done(0);
}

void camera::render(const hittable& world, const std::string& output_filename, unsigned num_threads) { 
    initialize();
    image img(image_width, image_height);

    auto start = std::chrono::high_resolution_clock::now();

    int rows_per_thread = image_height / num_threads;
    std::vector<std::thread> threads;

    for (unsigned int t = 0; t < num_threads; t++) {
        int y_start = t * rows_per_thread;
        int y_end = (t == num_threads - 1) ? image_height : y_start + rows_per_thread;

        threads.emplace_back([this, y_start, y_end, &world, &img]() {
            render_rows(y_start, y_end, world, img);
        });
    }

    while (lines_done.load() < image_height) {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        double progress = 100.0 * lines_done.load() / image_height;
        int progress_rounded = static_cast<int>(progress * 10 + 0.5);
        std::cerr << "\rProgress: " << (progress_rounded / 10) << '.' << (progress_rounded % 10)
                  << "% completed. " << "Elapsed time: " 
                  << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count()
                  << " seconds" << std::flush;
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cerr << "\rDone.\033[K\n";
    std::cerr << "Rendering completed in " << duration << " seconds\n";

    img.write_ppm(output_filename);
}

void camera::initialize() {
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = lookfrom;

    // Determine viewport dimensions
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / 2);
    auto viewport_height = 2.0 * h * focus_dist;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate the u, v, w unit vectors fro the camera coordinate frame
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    vec3 viewport_u = viewport_width * u;   // Vector across the horizontal viewport edge
    vec3 viewport_v = viewport_height * -v; // Vector down the vertical viewport edge

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Calculate defocus disk basis vectors
    auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

ray camera::get_ray(int i, int j) const {
    // Construct a camera ray originating from the defocus disk toward a random
    // point around the pixel location i, j.
    auto offset = sample_square();
    auto pixel_sample = pixel00_loc + (i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v;
    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();;
    auto ray_direction = pixel_sample - ray_origin;
    auto ray_time = random_double();

    return ray(ray_origin, ray_direction, ray_time);
}

vec3 camera::sample_square() const {
    // Returns the vector to a random point in the [-0.5, -0.5]-[0.5, 0.5] unit square
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

point3 camera::defocus_disk_sample() const {
    // Returns a random point on the defocus disk
    auto p = random_in_unit_disk();
    return center + p.x() * defocus_disk_u + p.y() * defocus_disk_v;
}

color camera::ray_color(const ray& r, int depth, const hittable& world) const {
    // If we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0) return color(0, 0, 0);
    
    hit_record rec;

    // If the ray hits nothing, return the background color
    if (!world.hit(r, interval(0.001, infinity), rec)) return background;

    ray scattered;
    color attenuation;
    color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat->scatter(r, rec, attenuation, scattered)) return color_from_emission;

    color color_from_scatter = attenuation * ray_color(scattered, depth - 1, world);

    return color_from_emission + color_from_scatter;
}

color camera::sample_pixel(int i, int j, const hittable& world) const {
    color pixel_color(0, 0, 0);
    for (int sample = 0; sample < samples_per_pixel; sample++) {
        ray r = get_ray(i, j);
        pixel_color += ray_color(r, max_depth, world);
    }
    pixel_color /= samples_per_pixel;

    return pixel_color;
}

void camera::render_rows(int y_start, int y_end, const hittable& world, image& img) const {
    // This thread renders the rows from y_start to y_end (exclusive)
    for (int y = y_start; y < y_end; y++) {
        for (int x = 0; x < image_width; x++) {
            img.at(x, y) = sample_pixel(x, y, world);
        }

        lines_done++; // Thread-safe atomic increment
    }
}
