#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "raytracer/color.hpp"

class image {
    public:
        int width;
        int height;
        std::vector<color> pixels;

        image(int w, int h) : width(w), height(h), pixels(w * h) {}

        color& at(int x, int y) { return pixels[y * width + x]; }

        const color& at(int x, int y) const { return pixels[y * width + x]; }

        void write_ppm(const std::string& filename) const {
            std::ofstream file(filename);
            if (!file) {
                std::cerr << "Error: Could not open file " << filename << "\n";
                return;
            }

            file << "P3\n" << width << ' ' << height << "\n255\n";

            for (int j = 0; j < height; j++) {
                for (int i = 0; i < width; i++) {
                    const color& pixel_color = at(i, j);

                    auto r = pixel_color.x();
                    auto g = pixel_color.y();
                    auto b = pixel_color.z();

                    // Apply a linear to gamma transform for gamma 2
                    r = linear_to_gamma(r);
                    g = linear_to_gamma(g);
                    b = linear_to_gamma(b);

                    // Translate the [0,1] component values to [0,255]
                    static const interval intensity(0.0, 0.999);
                    int rByte = static_cast<int>(256 * intensity.clamp(r));
                    int gByte = static_cast<int>(256 * intensity.clamp(g));
                    int bByte = static_cast<int>(256 * intensity.clamp(b));

                    file << rByte << ' ' << gByte << ' ' << bByte << '\n';
                }
            }

            file.close();
            std::cerr << "Image written to " << filename << "\n";
        }
};
