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
#include "camera.h"
#include "material.h"

color ray_color(const ray& r, const hittable_list &world, int depth) {
    hit_record rec;

    if (depth <= 0)
        return color(0,0,0);

    // t_min = 0.001 so rays don't collide with surface they were just reflected off of (called shadow acne)
    if( world.hit(r, 0.001, INF, rec ) )
    {
        ray scattered;
        color attenuation;
        if (rec.material->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1); 

        return color(0,0,0);

        // using random_unit_vector over random_in_unit_sphere results in more uniform scattering of light rays (fewer rays scattering toward the normal)
        // point3 target = rec.point + rec.normal + random_unit_vector();

        // alternative diffuse method that doesn't offset by surface's normal, about equivalent but has minor differences 
        // point3 target = rec.point + random_in_hemisphere(rec.normal);

        // -1 to 1 -> 0 to 1
        // return 0.5 * ray_color(ray(rec.point, target - rec.point), world, depth-1);
    }

    vec3 dir = unit_vector(r.direction());
    double t = 0.5 * (dir.y() + 1.0);
    // lerp
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main()
{
    image_buffer img(IMAGE_WIDTH, IMAGE_HEIGHT);

    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    point3 from(3,3,2);
    point3 to(0,0,-1);
    camera cam(from, to, vec3(0,1,0), 20, 2.0, (to-from).length());

    for( int y = IMAGE_HEIGHT - 1; y >= 0; y--)
    {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        for( int x = 0; x < IMAGE_WIDTH; x++)
        {
            color final_col(0, 0, 0);
            for (int s = 0; s < SAMPLES_PER_PIXEL; s++)
            {
                double u = (double(x) + random_double()) / (IMAGE_WIDTH - 1);
                double v = (double(y) + random_double()) / (IMAGE_HEIGHT - 1);
                ray ray = cam.get_ray(u, v);

                final_col += ray_color(ray, world, MAX_DEPTH);
            }

            img.write_color(x, y, final_col);
        }
    }

    std::cerr << "\nWriting PNG...\n";

    stbi_write_png("image.png", IMAGE_WIDTH, IMAGE_HEIGHT, STBI_rgb, img.buf, IMAGE_WIDTH * 3 * sizeof(img.buf[0]));

    std::cerr << "\nDone.\n";
}