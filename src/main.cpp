#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.hpp"

#define IMG_WIDTH(_var_w, _var_h, aspect_ratio, width) const int _var_w = (width); const int _var_h = static_cast<int>(_var_w / (aspect_ratio));
#define IMG_HEIGHT(_var_w, _var_h, aspect_ratio, height) const int _var_h = (height); const int _var_w = static_cast<int>(_var_h * (aspect_ratio));

#include "util/vec3.h"
#include "shader/ray_march_test_shader.h"
#include "render/renderer.h"
#include "shader/raymarch/scene.h"

constexpr int WORKER_COUNT = 96;

void init_scene(scene& scn) {
    scn.ambient_light = 0.15;

    auto ball_with_hole = new sdf_padded(
            new sdf_diff(
                    vec3(0, 0, 0),
                    new sdf_sphere(point3(0, 0, 0), .3),
                    new sdf_sphere(point3(-0.13, 0.2, 0.1), .4)
            ),
            0.1);
    ball_with_hole->set_diffuse_color(color(0.7, 0.3, 0.4));

    auto pole = new sdf_capsule(vec3(0, -1, 0), vec3(0, 1, 0), 0.05);
    pole->set_diffuse_color(color(0.7));

    auto complex = new sdf_union(vec3(0, 0, -2), ball_with_hole, pole);
    scn += complex;

//    auto small_ball = new sdf_sphere(point3(-0.6, 0.6, -1.22), .15);
//    small_ball->set_diffuse_color(color(0.1, 0.3, 0.8));
//    scn += small_ball;

    auto ground = new sdf_ground_plane(-1.0);
    ground->set_diffuse_color(color(0.15, 0.75, 0.3));
    scn += ground;

    auto cylinder = new sdf_cylinder(vec3(-2, -0.7, -4), 0.6, 0.2);
    cylinder->set_diffuse_color(color(0.7, 0.65, 0.3));
    scn += cylinder;

    auto capsule = new sdf_capsule(vec3(2, 0, -4), unit_vector(0.5, 2, 1.5), 0.5, 0.2);
    capsule->set_diffuse_color(color(0.8, 0.3, 0.95));
    scn += capsule;

    scn += new global_light_source(vec3(-2, -2, -1.5), 1);
//    scn += new point_light_source(vec3(0, 3, 0), 100, 15);
//    scn += new point_light_source(vec3(0, 3, 0), 1, 0);
}

int main() {
    // Image data
    IMG_HEIGHT(image_width, image_height, 16.0 / 9.0, 600)
    const int channels = 3;
    unsigned char img_data[image_width * image_height * channels];

    // Render thread function
    auto scn = scene();
    init_scene(scn);
    auto shader = new ray_march_test_shader(scn);

    auto render_job = [](frag_shader* shader, unsigned char* img_data, int start_index, int end_index) {
        return [shader, img_data, start_index, end_index]() {
            renderer render(shader);
            render.render_segment(img_data, image_width, image_height, start_index, end_index);
        };
    };

    // Create and start all worker threads
    std::vector<std::thread> workers;
    auto begin_time = std::chrono::steady_clock::now();
    int pixels_per_worker = static_cast<int>(image_width * image_height / WORKER_COUNT);
    for (int i = 0; i < WORKER_COUNT; i++) {
        int start = i * pixels_per_worker;
        int end = (i == WORKER_COUNT - 1) ? (image_height * image_width) : (start + pixels_per_worker);
        workers.push_back(std::thread(render_job(shader, img_data, start, end)));
    }

    // Wait for all threads to finish
    for (int i = 0; i < WORKER_COUNT; i++) {
        workers[i].join();
    }

    auto end_time = std::chrono::steady_clock::now();

    std::cout << "Render time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count()
        << "ms " << std::endl;

    // Write to png file
    begin_time = std::chrono::steady_clock::now();
    stbi_write_png("../out/image.png", image_width, image_height, channels, img_data, image_width * channels);
    end_time = std::chrono::steady_clock::now();

    std::cout << "File write time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count()
              << "ms " << std::endl;
}
