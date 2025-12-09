# Raytracer for ComS 336

## 1.0 Building

Run the build script for your operating system:

Linux/MacOS:
```bash
./scripts/build.sh
```

Windows:
```bat
./scripts/build.bat
```

## 2.0 Running

After building:
```bash
./build/raytracer --help
```
to see usage.

Example Usage:
```bash
./build/raytracer 2 > scene2_image.ppm
```
will output scene 2 into the output image scene2_image.ppm.

### 2.1 Textures

Place texture images in `images/` directory. Example:
```bash
images/earthmap.jpg
images/checker.png
```

Or set `RTW_IMAGES` environment variable to point to your texture directory:
```bash
export RTW_IMAGES=/path/to/textures
```

## 3.0 Cleaning

Run the clean script for your operating system:

Linux/MacOS:
```bash
./scripts/clean.sh
```

Windows:
```bat
./scripts/clean.bat
```

This will remove the build directory AND any .ppm files
