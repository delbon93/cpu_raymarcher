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

/**
 * Linearly interpolates between to values
 * @tparam T Any type that supports addition with itself and multiplication with double
 * @param a First value
 * @param b Second value
 * @param t Interpolation weight
 * @return Interpolated value = (1-t) * a + t * b
 */
template<typename T> inline T lerp(const T& a, const T& b, const double& t) { return (1-t)*a+t*b; }

/**
 * Clamps a value to a given range
 * @param t The value
 * @param lower Lower bounds of the range
 * @param upper Upper bounds of the range
 * @return Clamped value
 */
inline double clamp(const double& t, const double& lower, const double& upper) { return min(max(lower, t), upper);}

/**
 * Clamps a value to the normalized range [0-1]
 * @param t The value
 * @return Clamped value
 */
inline double nclamp(const double& t) { return clamp(t, 0.0, 1.0); }

/**
 * Smooth non-linear interpolation between two values
 * @param t Interpolation weight
 * @param lower
 * @param upper
 * @return
 */
inline double smoothstep(const double& t, const double& lower, const double& upper) {
    return nclamp((t - lower) / (upper - lower))
        * nclamp((t - lower) / (upper - lower))
        * (3 - 2 * nclamp((t - lower) / (upper - lower)));
}

/**
 * Smoothstep from 0 to 1
 * @param t Interpolation weight
 * @return
 */
inline double smoothstep(const double& t) { return smoothstep(t, 0.0, 1.0); }

#endif //RAYTRACING_IN_A_WEEKEND_MATH_H
