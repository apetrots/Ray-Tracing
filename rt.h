#ifndef RT
#define RT

#include <limits>
#include <cmath>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double PI = 3.1415926535897932385;
const double INF = std::numeric_limits<double>::infinity();

inline double degrees_to_rad(double degrees)
{
    return degrees * PI / 180.0;
}

#endif // RT