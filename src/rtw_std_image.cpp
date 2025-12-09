#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "raytracer/rtw_std_image.hpp"

rtw_image::rtw_image(const char* image_filename) {
    // Loads image data from the specified file. If the RTW_IMAGES environment variable is
    // defined, looks only in that directory for the image file. If the image was not found,
    // searches for the specified image file first from the current directory, then in the
    // images/ subdirectory, then the _parent's_ images/ subdirectory, and then _that_
    // parent, on so on, for six levels up. If the image was not loaded successfully,
    // width() and height() will return 0

    auto filename = std::string(image_filename);
    auto imagedir = std::getenv("RTW_IMAGES");

    // Hunt for the image file in various directories
    if (imagedir && load(std::string(imagedir) + "/" + image_filename)) return;
    else if (load(filename)) return;
    else if (load("images/" + filename)) return;
    else if (load("../images/" + filename)) return;
    else if (load("../../images/" + filename)) return;
    else if (load("../../../images/" + filename)) return;
    else if (load("../../../../images/" + filename)) return;
    else if (load("../../../../../images/" + filename)) return;

    std::cerr << "Error: Could not load image file '" << image_filename << "'\n";
}

rtw_image::~rtw_image() {
    delete[] bdata;
    STBI_FREE(fdata);
}

bool rtw_image::load(const std::string& filename) {
    // Loads the linear (gamma = 1) image data from the given file name. Returns true if the
    // load succeeded. The resulting data buffer contains the three [0.0, 1.0]
    // floating-point values for the first pixel (red, then green, then blue). Pixels are
    // contiguous, going left to right for the width of the image, followed by the next row
    // below, for the full height of the image

    auto n = bytes_per_pixel; // Dummy out parameter for original components per pixel
    fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
    if (!fdata) return false;

    bytes_per_scanline = image_width * bytes_per_pixel;
    convert_to_bytes();
    return true;
}

const unsigned char* rtw_image::pixel_data(int x, int y) const {
    // REturn the address of the 3 RGB bytes of the pixel at x, y. If there is no image
    // data loaded, returns magenta

    static unsigned char magenta[3] = { 255, 0, 255 };
    if (!bdata) return magenta;

    x = clamp(x, 0, image_width);
    y = clamp(y, 0, image_height);

    return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
}

int rtw_image::clamp(int x, int min, int max) const {
    if (x < min) return min;
    else if (x < max) return x;
    else return max - 1;
}

unsigned char rtw_image::float_to_byte(double val) const {
    if (val <= 0.0) return 0;
    else if (val >= 1.0) return 255;
    else return static_cast<unsigned char>(256.0 * val);
}
        
void rtw_image::convert_to_bytes() {
    // Convert the linear floating point pixel data to bytes, storing the
    // resulting byte data in the `bdata` member variable

    int total_bytes = image_width * image_height * bytes_per_pixel;
    bdata = new unsigned char[total_bytes];

    // Iterate through all pixel components, converting each from [0.0, 1.0]
    // float values to unsigned [0, 255] byte values

    auto *bptr = bdata;
    auto *fptr = fdata;
    for (auto i = 0; i < total_bytes; i++, bptr++, fptr++) {
        *bptr = float_to_byte(*fptr);
    }
}
