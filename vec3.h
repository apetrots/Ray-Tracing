#ifndef RT_VEC3
#define RT_VEC3

#include <cmath>
#include <iostream>

#include "rt.h"

using std::sqrt;

struct vec3 
{
    double e[3];

    vec3() : e{0, 0, 0} {}
    vec3(double x, double y, double z) : e{x, y, z} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    double r() const { return e[0]; }
    double g() const { return e[1]; }
    double b() const { return e[2]; }

    inline static vec3 random() 
    {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random_range(double min, double max)
    {
        return vec3(random_double_range(min, max), random_double_range(min, max), random_double_range(min, max));
    }

    bool near_zero() const {
        // Returns true if vector is close to 0 in all dimensions
        const auto epsilon = 1e-8;
        return (fabs(e[0]) < epsilon) && (fabs(e[1]) < epsilon) && (fabs(e[2]) < epsilon); 
    }

    // negate
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v) 
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }
    vec3& operator*=(const double t) 
    {
        e[0] *= t; 
        e[1] *= t; 
        e[2] *= t; 
        return *this;
    }
    vec3& operator/=(const double t) { return (*this *= 1/t); }

    double length() const { return sqrt(length_squared()); }
    double length_squared() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

vec3 random_in_unit_sphere()
{
    while (true) {
        vec3 p = vec3::random_range(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

// Gets random point within unit sphere, then normalizes it to get to being on the unit sphere 
vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    return dot(in_unit_sphere, normal) > 0.0 ? // in same hemisphere as normal?
        in_unit_sphere :
        -in_unit_sphere;

}

vec3 reflect(const vec3& dir_in, const vec3& normal)
{
    return dir_in - 2 * dot(dir_in, normal) * normal;
}

vec3 refract(const vec3& uv, const vec3& normal, double etai_over_etat)
{
    double cos_theta = fmin(dot(-uv, normal), 1.0f);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*normal);
    vec3 r_out_para = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * normal;
    return r_out_perp + r_out_para;
}

// for use with thin lens approximation from depth of field
vec3 random_in_unit_disk()
{
    while (true)
    {
        vec3 p(random_double_range(-1, 1), random_double_range(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

#endif // RT_VEC3