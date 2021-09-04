#ifndef RT_MATERIAL
#define RT_MATERIAL

#include "rt.h"
#include "ray.h"
#include "hittable.h"

struct material
{
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

// lambertian diffuse material, can either scatter always and attenuate by reflectance R or it can scatter w/ no attenuation but absorb 1 - R of the rays (or a mix of those two approaches)
struct lambertian : public material 
{
    color albedo;
    
    lambertian(const color& a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override 
    {
        vec3 scatter_dir = rec.normal + random_unit_vector();
    
        // check done in order to avoid NaNs and infinity mischief when summing two random unit vectors that could be potentially be opposites (summing to 0, so it has a zero scatter direction vector)
        if (scatter_dir.near_zero())
            scatter_dir = rec.normal;

        scattered = ray(rec.point, scatter_dir);
        attenuation = albedo;
        return true;
    }

};

struct metal : public material
{
    color albedo;
    float fuzz;

    metal(const color& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override 
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.point, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

struct dielectric : public material
{
    double ir; // Index of refraction

    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override 
    {
        attenuation = color(1.0, 1.0, 1.0);
        double refract_ratio = rec.front_face ? (1.0 / ir) : ir;
        
        vec3 unit_dir = unit_vector(r_in.direction());
        vec3 refracted = refract(unit_dir, rec.normal, refract_ratio);

        double cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        // To solve the possibility of total internal reflection where the sin(theta) of the refracted ray is calculated to be over 1  (which is impossible). In other words, when the angle goes over critical angle where the refracted ray turns into a reflected ray on the other side of the normal, total internal reflection occurs (happens whenever the angle of incidence is large enough).
        bool total_internal_reflection = refract_ratio * sin_theta > 1.0;
        vec3 refract_dir;
        // Reflectivity varies w/ angle (i.e. looking at a window at a steep angle makes it a mirror). There is a formal, complex equation to simulate that, however "everybody" uses Schlick's Approximation to get a suprisingly accurate polynomial approximation of it 
        if (total_internal_reflection || reflectance(cos_theta, refract_ratio) > random_double())
            refract_dir = reflect(unit_dir, rec.normal);
        else
            refract_dir = refract(unit_dir, rec.normal, refract_ratio);

        scattered = ray(rec.point, refract_dir);
        return true;
    }
};

#endif // RT_MATERIAL