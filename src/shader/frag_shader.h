#ifndef RAYTRACING_IN_A_WEEKEND_FRAG_SHADER_H
#define RAYTRACING_IN_A_WEEKEND_FRAG_SHADER_H

#include "../util/vec3.h"

class frag_shader {
public:
    virtual color frag(const vec3& uv) = 0;
    virtual ~frag_shader() {};

    virtual color clear_color(const vec3& uv) {
        auto t = uv.y();
        return (1.0-t)*color(1.0, 1.0, 1.0) + t * color(0.4, 0.7, 1.0);
    }
};

#endif //RAYTRACING_IN_A_WEEKEND_FRAG_SHADER_H
