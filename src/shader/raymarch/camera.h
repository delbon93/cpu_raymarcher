//
// Created by david on 07.05.2021.
//

#ifndef RAYTRACING_IN_A_WEEKEND_CAMERA_H
#define RAYTRACING_IN_A_WEEKEND_CAMERA_H

#include "../../util/vec3.h"
#include "ray.h"

/**
 * A camera that exists in world space. Provides rays for given UV viewport coordinates
 */
class camera {
public:
    /**
     * Creates a camera
     * @param _origin The origin point of the camera in world space
     * @param _viewport_height Height of the viewport in world units
     * @param aspect_ratio Aspect ratio of the viewport
     * @param _focal_length Distance of the viewport from the cameras origin into negative z direction
     */
    camera(const point3& _origin, const double _viewport_height, const double aspect_ratio,
           const double _focal_length)
        : origin(_origin), viewport_height(_viewport_height), focal_length(_focal_length) {
        viewport_width = aspect_ratio * viewport_height;
        horizontal = vec3(viewport_width, 0, 0);
        vertical = vec3(0, viewport_height, 0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    }

    /**
     * Creates a ray from the given viewport UV coordinate into the scene
     * @param uv UV viewport coordinate
     * @param normalized Whether or not the ray's direction vector should be normalized
     * @return The created ray
     */
    ray get_ray(const vec3& uv, bool normalized) const {
        double origin_height = viewport_height * 0.7;
        double origin_width = origin_height * viewport_width / viewport_height;
        vec3 origin_hor = vec3(origin_width, 0, 0);
        vec3 origin_ver = vec3(0, origin_height, 0);
        vec3 origin_lower_left = origin - origin_hor / 2 - origin_ver / 2;

        vec3 ray_origin = origin_lower_left + uv.x() * origin_hor + uv.y() * origin_ver;
        vec3 dir = lower_left_corner + uv.x() * horizontal + uv.y() * vertical - ray_origin;
        if (normalized) dir = unit_vector(dir);

        return ray(ray_origin, dir);
    }

private:
    double viewport_width;
    double viewport_height;
    double focal_length;
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

#endif //RAYTRACING_IN_A_WEEKEND_CAMERA_H
