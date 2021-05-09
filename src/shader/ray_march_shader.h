#ifndef RAYTRACING_IN_A_WEEKEND_RAY_MARCH_SHADER_H
#define RAYTRACING_IN_A_WEEKEND_RAY_MARCH_SHADER_H

#include "frag_shader.h"
#include "raymarch/camera.h"
#include "raymarch/objects.h"

/**
 * Information about a completed raycast:
 */
struct raycast_info {
    /**
     * The point that was closest to an sdf_object along the ray. Semantically identical to the point of
     * collision if the ray hit an object
     */
    point3 hitpoint;

    /**
     * Pointer to the sdf_object that was hit in case of a collision. 'nullptr' if no object was hit
     */
    sdf_object* target = nullptr;

    /**
     * The distance from 'hitpoint' to the closest sdf_object at that location
     */
    double min_dist {};
};

/**
 * Ray-marching fragment shader
 */
class ray_march_shader : public frag_shader {
public:
    ray_march_shader();
    color frag(const vec3 &uv) override;

private:
    /**
     * Performs a raycast to probe information about the scene
     * @param r Ray to be cast
     * @param ignore Pointer to an sdf_object that will not be considered in the raycast
     * @return Information about the completed raycast
     */
    raycast_info raycast(const ray& r, sdf_object* ignore = nullptr, double min_travel = 0.0) const;

private:
    camera cam = camera(vec3(), 2.0, 16.0 / 9.0, 1.0);
    scene scn;
};

#endif //RAYTRACING_IN_A_WEEKEND_RAY_MARCH_SHADER_H
