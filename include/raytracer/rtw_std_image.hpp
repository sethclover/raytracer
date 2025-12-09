#pragma once

// Disable strict warning for this header when using MSVC
#ifdef _MSC_VER
    #pragma warning(push, 0)
#endif

#define STB_FAILURE_USERMSG
#include "third_party/stb_image.h"

#include <string>

class rtw_image {
    public:
        rtw_image() = default;
        rtw_image(const char* image_filename);

        ~rtw_image();

        bool load(const std::string& filename);
        int width() const { return !fdata ? 0 : image_width; }
        int height() const { return !fdata ? 0 : image_height; }
        const unsigned char* pixel_data(int x, int y) const;

    private:
        const int bytes_per_pixel = 3;  // RGB
        float* fdata = nullptr;         // Linear float point pixel data
        unsigned char* bdata = nullptr; // Linear 8-bit per pixel data
        int image_width = 0;            // Loaded image width
        int image_height = 0;           // Loaded image height
        int bytes_per_scanline = 0;     // Loaded image bytes per scanline
        
        int clamp(int x, int min, int max) const;
        unsigned char float_to_byte(double val) const;
        void convert_to_bytes();
    };

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
