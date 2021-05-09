#ifndef RAYTRACING_IN_A_WEEKEND_MATH_H
#define RAYTRACING_IN_A_WEEKEND_MATH_H

#include <cmath>
#include "vec3.h"

#define max std::max
#define min std::min
#define sqrt std::sqrt
#define pow std::pow
#define exp std::exp
#define abs std::abs

template<typename T> inline T lerp(const T& a, const T& b, const double& t) { return (1-t)*a+t*b; }

inline double clamp(const double& t, const double& lower, const double& upper)
    { return min(max(lower, t), upper);}

inline double nclamp(const double& t) { return clamp(t, 0.0, 1.0); }

inline double smoothstep(const double& t, const double& lower, const double& upper) {
    return nclamp((t - lower) / (upper - lower))
        * nclamp((t - lower) / (upper - lower))
        * (3 - 2 * nclamp((t - lower) / (upper - lower)));
}

inline double smoothstep(const double& t) { return smoothstep(t, 0.0, 1.0); }

template<typename T> inline T slerp(const T& a, const T& b, const double& t) { return lerp(a, b, smoothstep(t)); }
template<typename T>
inline T slerp(const T& a, const T& b, const double& t, const double& lower, const double& upper)
    { return lerp(a, b, smoothstep(t, lower, upper)); }

inline vec3 clamp(vec3 v, double lower, double upper) {
    return vec3(clamp(v.x(), lower, upper), clamp(v.y(), lower, upper), clamp(v.z(), lower, upper));
}


#endif //RAYTRACING_IN_A_WEEKEND_MATH_H
