# Raytracer

A physically-based raytracer implementation by Seth Clover for ComS 336, supporting multiple materials, textures, and scene configurations.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Building](#building)
- [Usage](#usage)
- [Scene Gallery](#scene-gallery)
- [Dependencies](#dependencies)
- [Troubleshooting](#troubleshooting)
- [Cleaning](#cleaning)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Features

### Required:
- Camera w/ configurable position, orientation, and FOV
- Anti-aliasing
- Ray/sphere intersections
- Ray/triangle intersections
- Texture loading (using stb_image)
- Textured spheres and triangles
- Mesh loading (using tinyobjloader for OBJ files)
- Spatial subdivision acceleration structure (BVH)
- Materials:
  - Lambertian (diffuse)
  - Metal (specular)
  - Dielectric (glass)
- Emissive materials (lights)

### Additional:
- Volume rendering (10)
- Quads (10)
- Motion blur (10)
- Depth of field (10)
- Object instancing (10)
- Perlin noise (10)
- Parallelization using multi-threaded row rendering (10)

Total additional points: 70

## Prerequisites

- **C++ Compiler**: C++20 or later support required
  - Windows: MSVC 2019+ or MinGW-w64 (GCC 10+)
  - Linux/macOS: GCC 10+ or Clang 11+
- **CMake**: Version 3.15 or higher

## Building

### Windows

```bat
scripts\build.bat
```

The executable will be placed in `build/Release/raytracer.exe`.

### Linux/macOS

```bash
chmod +x scripts/build.sh
./scripts/build.sh
```

The executable will be placed in `build/raytracer`.

### Build Options

**Debug build** (slower, with debugging symbols):
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

**Release build** (optimized, default in scripts):
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Usage

### Basic Usage

```bash
# Display help
./build/raytracer --help

# Render scene 1 with default thread count
./build/raytracer 1

# Render scene 2 with 8 threads (will cap at hardware concurrency)
./build/raytracer 2 --threads 8
./build/raytracer 2 -t 8
```

### Output

Images are saved to `images/<scene_name>_<number>.ppm`

Example: Scene 2 outputs to `images/checkered_spheres_2.ppm`

### Converting PPM to PNG

```bash
# Using ImageMagick
magick images/scene_1.ppm images/scene_1.png

# Using GIMP (GUI)
# File -> Open -> Select .ppm -> Export As -> .png
```

## Scene Gallery

| Scene # | Name | Description |
|---------|------|-------------|
| 1 | Bouncing Spheres | Large Spheres and random spheres with motion blur |
| 2 | Checkered Spheres | Spatial checker texture spheres |
| 3 | Earth | Image-mapped sphere in space |
| 4 | Perlin Spheres | Marble sphere on marble floor using Perlin noise |
| 5 | Quads | Quadrilateral primitives in box shape |
| 6 | Octohedron | Octohedron made of 8 triangles |
| 7 | Simple Light | Scene 4 (Perlin Spheres) with emissive materials |
| 8 | Cornell Box | Classic Cornell box scene |
| 9 | Cornell Smoke | Classic Cornell box scene with volumetric rendering |
| 10 | Next Week | Complex scene from Ray Tracing in One Weekend series |
| 11 | Triangle Mesh |  Red teapot OBJ model |
| 12 | Final Render | Final showcase scene with all features (and Homer) |

## Dependencies

All dependencies are header-only and included in the repository:

- **stb_image**: Image loading (public domain)
- **tinyobjloader**: OBJ mesh loading (MIT license)

## Troubleshooting

### Texture file not found

```
Error: Could not load image file 'earthmap.jpg'
```

**Solution**: 
1. Place texture files in `images/` directory, or
2. Set the `RTW_IMAGES` environment variable:

```bash
# Linux/macOS
export RTW_IMAGES=/path/to/textures

# Windows (PowerShell)
$env:RTW_IMAGES="C:\path\to\textures"

# Windows (CMD)
set RTW_IMAGES=C:\path\to\textures
```

### OBJ model not found

Ensure `.obj` files are in the `models/` directory:
```bash
models/
├── teapot.obj
└── homer.obj
```

### Slow rendering

- Reduce samples per pixel, image resolution, or max ray depth in `src/scenes.cpp` for the scene being rendered.

## Cleaning

Remove build artifacts:

### Windows
```bat
scripts\clean.bat
```

### Linux/macOS
```bash
./scripts/clean.sh
```

## License

Educational project for ComS 336. Based on the [Ray Tracing in One Weekend](https://raytracing.github.io/) series by Peter Shirley.

## Acknowledgments

- Peter Shirley's *Ray Tracing in One Weekend* book series
- stb_image by Sean Barrett
- tinyobjloader by Syoyo Fujita