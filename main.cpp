#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "vec3.h"
#include "image.h"

int main()
{
    const int image_width = 256;
    const int image_height = 256;

    image_buffer img(image_width, image_height);

    for( int y = image_height - 1; y >= 0; y--)
    {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        for( int x = 0; x < image_width; x++)
        {
            double r = double(x) / (image_width - 1);
            double g = double(y) / (image_height - 1);
            double b = 0.25;

            color col(
                r,
                g,
                b
            );

            img.write_color(x, y, col);
        }
    }

    std::cerr << "\nWriting PNG...\n";

    stbi_write_png("image.png", image_width, image_height, STBI_rgb, img.buf, img.image_width * 3 * sizeof(img.buf[0]));
 
    std::cerr << "\nDone.\n";
}