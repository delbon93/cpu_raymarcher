#ifndef RAYTRACING_IN_A_WEEKEND_RAY_MARCH_SHADER_H
#define RAYTRACING_IN_A_WEEKEND_RAY_MARCH_SHADER_H

#include "frag_shader.h"
#include "raymarch/camera.h"
#include "raymarch/objects.h"

struct raycast_info {
    point3 hitpoint;
    sdf_object* target{};
    double min_dist{};
};

class ray_march_shader : public frag_shader {
public:
    ray_march_shader();
    color frag(const vec3 &uv) override;

private:
    raycast_info raycast(const ray& r, sdf_object* ignore) const;

private:
    camera cam = camera(vec3(), 2.0, 16.0 / 9.0, 1.0);
    scene scn;
};

#endif //RAYTRACING_IN_A_WEEKEND_RAY_MARCH_SHADER_H
