#ifndef RAYTRACING_IN_A_WEEKEND_RENDERER_H
#define RAYTRACING_IN_A_WEEKEND_RENDERER_H

#include "../util/vec3.h"
#include "../shader/frag_shader.h"

/**
 * Renderer that renders pixel fragments into a heap allocated array segment of RGB data. It uses the
 * given shader type's fragment function
 * @tparam shader_type Used shader type
 */
template<class shader_type>
class renderer {
public:
    renderer() {
        shader = new shader_type();
    }

    /**
     * Renders pixel data into a 24-bit RGB buffer
     * @param target_data Pointer to beginning of the buffer
     * @param target_width Width of the complete image to be rendered
     * @param target_height Height of the complete image to be rendered
     * @param begin_pixel Pixel index of the first pixel of the segment to be rendered
     * @param end_pixel Pixel index of the first pixel AFTER the segment to be rendered
     */
    void render_segment(unsigned char* target_data, int target_width, int target_height, int begin_pixel, int end_pixel) {
        int pixel_index = begin_pixel;
        while(pixel_index < end_pixel) {
            double ux = double(pixel_index % target_width) / target_width; // NOLINT(bugprone-integer-division)
            double uy = 1.0-double(pixel_index / target_width) / target_height; // NOLINT(bugprone-integer-division)
            write_color(target_data, pixel_index * 3, shader->frag(vec3(ux, uy, 0)));
            pixel_index++;
        }
    }

    ~renderer() {
        delete shader;
    }

private:
    /**
     * Shorthand function to write a single 24-bit RGB pixel into a buffer
     * @param img_data Pointer to beginning of the buffer
     * @param index Byte index of the first byte of the RGB pixel data
     * @param pixel_color RGB pixel color to be stored
     */
    void write_color(unsigned char* img_data, int index, color pixel_color) {
        unsigned char r = static_cast<unsigned char>(255.999 * pixel_color.x());
        unsigned char g = static_cast<unsigned char>(255.999 * pixel_color.y());
        unsigned char b = static_cast<unsigned char>(255.999 * pixel_color.z());

        img_data[index + 0] = r;
        img_data[index + 1] = g;
        img_data[index + 2] = b;
    }

private:
    frag_shader* shader;

};

#endif //RAYTRACING_IN_A_WEEKEND_RENDERER_H
