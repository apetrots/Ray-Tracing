#ifndef RT_COLOR
#define RT_COLOR

#include <iostream>

#include "vec3.h"
#include "rt.h"

// individual size of each component in a pixel (R, G, B)
using pixel_comp = uint8_t;
const uint8_t PIXEL_COMP_COUNT = 3; // RGB
const uint32_t SAMPLES_PER_PIXEL = 100;

struct image_buffer
{
    uint32_t image_width;
    uint32_t image_height;

    size_t buf_size;
    pixel_comp* buf;

    image_buffer( uint32_t _image_width, uint32_t _image_height ) 
    {
        image_width = _image_width;
        image_height = _image_height;
        buf_size = (image_width * image_height) * PIXEL_COMP_COUNT;
        buf = new pixel_comp[buf_size];
    }

    void write_color( uint32_t x, uint32_t y, color col )
    {
        // gamma correction of 2 involves the power of 1/gamma or 1/2, so we can just sqrt
        auto scale = 1.0 / SAMPLES_PER_PIXEL;
        auto r = sqrt(col.r() * scale);
        auto g = sqrt(col.g() * scale);
        auto b = sqrt(col.b() * scale);

        uint32_t idx = ((image_height - 1 - y) * image_width + x);
        buf[idx * PIXEL_COMP_COUNT] = static_cast<pixel_comp>(255 * clamp(r, 0.0, 0.999));
        buf[idx * PIXEL_COMP_COUNT + 1] = static_cast<pixel_comp>(255 * clamp(g, 0.0, 0.999));
        buf[idx * PIXEL_COMP_COUNT + 2] = static_cast<pixel_comp>(255 * clamp(b, 0.0, 0.999));
    }
};

#endif // RT_COLOR