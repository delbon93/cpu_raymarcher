#ifndef RAYTRACING_IN_A_WEEKEND_RENDERER_H
#define RAYTRACING_IN_A_WEEKEND_RENDERER_H

#include "../util/vec3.h"
#include "../shader/frag_shader.h"

template<class shader_type>
class renderer {
public:
    renderer() {
        shader = new shader_type();
    }

    void render_segment(unsigned char* target_data, int target_width, int target_height, int begin_pixel, int end_pixel) {
        int pixel_index = begin_pixel;
        while(pixel_index < end_pixel) {
            double ux = double(pixel_index % target_width) / target_width; // NOLINT(bugprone-integer-division)
            double uy = 1.0-double(pixel_index / target_width) / target_height; // NOLINT(bugprone-integer-division)
            write_color(target_data, pixel_index * 3, shader->frag(vec3(ux, uy, 0)));
            pixel_index++;
        }
    }

    void write_color(unsigned char* img_data, int index, color pixel_color) {
        unsigned char r = static_cast<unsigned char>(255.999 * pixel_color.x());
        unsigned char g = static_cast<unsigned char>(255.999 * pixel_color.y());
        unsigned char b = static_cast<unsigned char>(255.999 * pixel_color.z());

        img_data[index + 0] = r;
        img_data[index + 1] = g;
        img_data[index + 2] = b;
    }


    ~renderer() {
        delete shader;
    }
private:
    frag_shader* shader;

};

#endif //RAYTRACING_IN_A_WEEKEND_RENDERER_H
