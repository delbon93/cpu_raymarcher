#ifndef RAYTRACING_IN_A_WEEKEND_FRAG_SHADER_H
#define RAYTRACING_IN_A_WEEKEND_FRAG_SHADER_H

#include "../util/vec3.h"

/**
 * Abstract fragment shader class
 */
class frag_shader {
public:
    /**
     * Returns a 24-bit RGB pixel color for the given UV coordinates of the image
     * @param uv UV coordinates of the image
     * @return Pixel color
     */
    virtual color frag(const vec3& uv) = 0;

    virtual ~frag_shader() {};

protected:
    /**
     * Returns the clear color for the given UV coordinate
     * @param uv UV coordinates of the image
     * @return Clear color
     */
    virtual color clear_color(const vec3& uv) {
        auto t = uv.y();
        return (1.0-t)*color(1.0, 1.0, 1.0) + t * color(0.4, 0.7, 1.0);
    }
};

#endif //RAYTRACING_IN_A_WEEKEND_FRAG_SHADER_H
