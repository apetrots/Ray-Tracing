#ifndef RT
#define RT

#include <limits>
#include <cmath>
#include <memory>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double ASPECT_RATIO = 16.0 / 9.0;
const int IMAGE_WIDTH = 400;
const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);
const int MAX_DEPTH = 50; // how deep should we go in raycast bounces?

const double PI = 3.1415926535897932385;
const double INF = std::numeric_limits<double>::infinity();

inline double degrees_to_rad(double degrees)
{
    return degrees * PI / 180.0;
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// [0, 1)
inline double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double_range(double min, double max)
{
    return min + (max - min) * random_double();
}

#endif // RT