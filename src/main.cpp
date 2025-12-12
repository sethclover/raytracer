#include <filesystem>
#include <iostream>
#include <string>

#include "scenes.hpp"

// -h for help or number to choose from scenes
static const std::string usage = "Usage: raytracer [-h | --help | <scene_number>]\n"
                                          "  -h, --help       Show this help message\n"
                                          "  <scene_number>   Scene to render (1-" + std::to_string(SCENE_COUNT) + ")\n";

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

    // Check if images directory exists
    std::filesystem::path images_dir("images");
    if (!std::filesystem::exists(images_dir)) {
        std::cerr << "Error: 'images' directory not found. Please create one or run from the project root.\n";
        return 1;
    }

    if (scene_choice == 1) {
        bouncing_spheres_1();
    } 
    else if (scene_choice == 2) {
        checkered_spheres_2();
    } 
    else if (scene_choice == 3) {
        earth_3();
    }
    else if (scene_choice == 4) {
        perlin_spheres_4();
    }
    else if (scene_choice == 5) {
        quads_5();
    }
    else if (scene_choice == 6) {
        octohedron_6();
    }
    else if (scene_choice == 7) {
        simple_light_7();
    }
    else if (scene_choice == 8) {
        cornell_box_8();
    }
    else if (scene_choice == 9) {
        cornell_smoke_9();
    }
    else if (scene_choice == 10) {
        next_week_10();
    }

    return 0;
}
