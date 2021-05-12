#ifndef CPU_RAYMARCHER_RAY_MARCH_TEST_SHADER_H
#define CPU_RAYMARCHER_RAY_MARCH_TEST_SHADER_H

#include "ray_march_shader.h"

class ray_march_test_shader : public ray_march_shader {
public:
    explicit ray_march_test_shader(const scene& _scn) : ray_march_shader(_scn) {}
protected:

    void frag_ray(raycast_info r_info, color& out_col) override;
};


#endif //CPU_RAYMARCHER_RAY_MARCH_TEST_SHADER_H
