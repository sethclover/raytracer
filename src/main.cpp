#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

#include "scenes.hpp"

static const std::string usage = "Usage: raytracer [-h | --help] <scene_number> [-t N | --threads N]\n"
                                          "  -h, --help           Show this help message\n"
                                          "  <scene_number>       Scene to render (1-" + std::to_string(SCENE_COUNT) + ")\n"
                                          "  -h N, --threads N    Number of threads to use (default: auto-detect)\n";

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error: Expected at least 1 argument.\n" << usage;
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
    } catch (const std::exception& e [[maybe_unused]]) {
        std::cerr << "Error: Invalid scene number '" << arg1 << "'.\n" << usage;
        return 1;
    }

    if (scene_choice == 0 || scene_choice > SCENE_COUNT) {
        std::cerr << "Error: Scene number out of range.\n" << usage;
        return 1;
    }

    unsigned num_threads = std::thread::hardware_concurrency(); // default
    if (argc >= 4 && (std::string(argv[2]) == "-t" || std::string(argv[2]) == "--threads")) {
        try {
            num_threads = std::stoul(argv[3]);
            if (num_threads == 0) {
                std::cerr << "Error: Thread count must be positive.\n" << usage;
                return 1;
            }
        } catch (const std::exception& e [[maybe_unused]]) {
            std::cerr << "Error: Invalid thread count '" << argv[3] << "'.\n" << usage;
            return 1;
        }
    }
    std::cout << "Using " << num_threads << " threads for rendering.\n";

    // Check if images directory exists
    std::filesystem::path images_dir("images");
    if (!std::filesystem::exists(images_dir)) {
        std::cerr << "Error: 'images' directory not found. Please create one or run from the project root.\n";
        return 1;
    }

    if (scene_choice == 1) {
        std::cout << "Rendering scene 1: Bouncing Spheres\n";
        bouncing_spheres_1(num_threads);
    } 
    else if (scene_choice == 2) {
        std::cout << "Rendering scene 2: Checkered Spheres\n";
        checkered_spheres_2(num_threads);
    } 
    else if (scene_choice == 3) {
        std::cout << "Rendering scene 3: Earth\n";
        earth_3(num_threads);
    }
    else if (scene_choice == 4) {
        std::cout << "Rendering scene 4: Perlin Spheres\n";
        perlin_spheres_4(num_threads);
    }
    else if (scene_choice == 5) {
        std::cout << "Rendering scene 5: Quads\n";
        quads_5(num_threads);
    }
    else if (scene_choice == 6) {
        std::cout << "Rendering scene 6: Octohedron\n";
        octohedron_6(num_threads);
    }
    else if (scene_choice == 7) {
        std::cout << "Rendering scene 7: Simple Light\n";
        simple_light_7(num_threads);
    }
    else if (scene_choice == 8) {
        std::cout << "Rendering scene 8: Cornell Box\n";
        cornell_box_8(num_threads);
    }
    else if (scene_choice == 9) {
        std::cout << "Rendering scene 9: Cornell Smoke\n";
        cornell_smoke_9(num_threads);
    }
    else if (scene_choice == 10) {
        std::cout << "Rendering scene 10: Next Week\n";
        next_week_10(num_threads);
    }
    else if (scene_choice == 11) {
        std::cout << "Rendering scene 11: Triangle Mesh\n";
        triangle_mesh_11(num_threads);
    }
    else if (scene_choice == 12) {
        std::cout << "Rendering scene 12: Final Render\n";
        final_render_12(num_threads);
    }

    return 0;
}
