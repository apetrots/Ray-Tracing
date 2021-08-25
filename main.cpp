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
#include "rt.h"
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"

color ray_color(const ray& r, const hittable_list &world) {
    hit_record rec;
    if( world.hit(r, 0, INF, rec ) )
    {
        // -1 to 1 -> 0 to 1
        return 0.5 * (rec.normal + color(1,1,1));
    }

    vec3 dir = unit_vector(r.direction());
    double t = 0.5 * (dir.y() + 1.0);
    // lerp
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


int main()
{
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    point3 origin(0, 0, 0);
    vec3 right(viewport_width, 0, 0);
    vec3 up(0, viewport_height, 0);
    point3 btm_left = origin - (right / 2) - (up / 2) - vec3(0, 0, focal_length);

    image_buffer img(image_width, image_height);

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    for( int y = image_height - 1; y >= 0; y--)
    {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        for( int x = 0; x < image_width; x++)
        {
            double u = double(x) / (image_width - 1);
            double v = double(y) / (image_height - 1);
            ray r(origin, btm_left + u * right + v * up - origin);

            color col = ray_color(r, world);

            img.write_color(x, y, col);
        }
    }

    std::cerr << "\nWriting PNG...\n";

    stbi_write_png("image.png", image_width, image_height, STBI_rgb, img.buf, img.image_width * 3 * sizeof(img.buf[0]));

    std::cerr << "\nDone.\n";
}