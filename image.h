#ifndef RT_COLOR
#define RT_COLOR

#include <iostream>

#include "vec3.h"

// individual size of each component in a pixel (R, G, B)
using pixel_comp = uint8_t;
const uint8_t pixel_comp_count = 3; // RGB

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
        buf_size = (image_width * image_height) * pixel_comp_count;
        buf = new pixel_comp[buf_size];
    }

    void write_color( uint32_t x, uint32_t y, color col )
    {
        buf[(y * image_width + x) * pixel_comp_count] = static_cast<pixel_comp>(255 * col.x());
        buf[(y * image_width + x) * pixel_comp_count + 1] = static_cast<pixel_comp>(255 * col.y());
        buf[(y * image_width + x) * pixel_comp_count + 2] = static_cast<pixel_comp>(255 * col.z());
    }
};

#endif // RT_COLOR