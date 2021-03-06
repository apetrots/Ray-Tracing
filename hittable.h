#ifndef RT_HITTABLE
#define RT_HITTABLE

#include "ray.h"

struct material;

struct hit_record
{
    point3 point;
    vec3 normal;
    shared_ptr<material> material;
    float time;
    bool front_face;
    
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

struct hittable
{
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif // RT_HITTABLE