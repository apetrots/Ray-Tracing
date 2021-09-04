#ifndef RT_CAMERA
#define RT_CAMERA

#include "rt.h"

struct camera 
{
    point3 origin;
    point3 btm_left_pt;
    vec3 right; 
    vec3 up;
    double viewport_height;
    double viewport_width;
    double lens_radius;
    // defines coordinate system local to camera
    vec3 u, v, w;

    camera(
        point3 _origin,
        point3 look_at,
        vec3 _up,
        double vert_fov_deg,
        double aperture,
        double focus_dist
    )
    {
        double theta = degrees_to_rad(vert_fov_deg);
        double half_height = tan(theta/2); // considering that adjacent side is 1 (i.e. viewing z=-1 plane)
        viewport_height = 2.0 * half_height;
        viewport_width = ASPECT_RATIO * viewport_height;

        w = unit_vector(_origin - look_at);
        u = unit_vector(cross(_up, w));
        v = cross(w, u);

        origin = _origin;
        right = focus_dist * viewport_width * u;
        up = focus_dist * viewport_height * v;
        btm_left_pt = origin - (right / 2.0f) - (up / 2.0f) - focus_dist*w;

        lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(origin + offset, btm_left_pt + s*right + t*up - origin - offset);
    } 
};

#endif // RT_CAMERA