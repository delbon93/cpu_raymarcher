#ifndef RAYTRACING_IN_A_WEEKEND_RAY_H
#define RAYTRACING_IN_A_WEEKEND_RAY_H

#include "../../util/vec3.h"

/**
 * Ray class, contains an origin point and a direction vector
 */
class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    /**
     * @return Origin of the ray
     */
    point3 origin() const { return orig; }
    /**
     * @return Direction of the ray
     */
    vec3 direction() const { return dir; }

    /**
     * @param t Distance from the ray origin in world units
     * @return Point on the ray t units away from the origin
     */
    point3 at(double t) const {
        return orig + t * dir;
    }
private:
    point3 orig;
    vec3 dir;
};

#endif //RAYTRACING_IN_A_WEEKEND_RAY_H
