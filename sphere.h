#ifndef RT_SPHERE
#define RT_SPHERE

#include "hittable.h"

struct sphere : public hittable
{
    sphere() {}
    sphere(point3 _center, double _radius) : center(_center), radius(_radius) { }

    point3 center;
    double radius;

    bool hit(const ray& ray, double t_min, double t_max, hit_record& rec) const override;
};


bool sphere::hit(const ray& ray, double t_min, double t_max, hit_record& rec) const 
{
    // quadratic equation
    vec3 diff = ray.origin() - center;
    double a = ray.direction().length_squared();
    double half_b = dot(diff, ray.direction());
    double c = diff.length_squared() - radius * radius;
    double discriminant = half_b*half_b - a*c;
    // discriminant: positive = two real solutions, zero = one real solution,  negative = no real solutions 
    if (discriminant < 0)
    {
        return false;
    }

    double sqrtDisc = sqrt(discriminant);

    double root = (-half_b - sqrtDisc) / a;

    // viewing distance between t_min and t_max
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtDisc) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.time = root;
    rec.point = ray.at(rec.time);
    vec3 outward_normal = (rec.point - center) / radius;

    rec.set_face_normal(ray, outward_normal);

    return true;
}



#endif // RT_SPHERE