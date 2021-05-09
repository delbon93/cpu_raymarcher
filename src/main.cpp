#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/std_image_write.h"

#define IMG_WIDTH(_var_w, _var_h, aspect_ratio, width) const int _var_w = (width); const int _var_h = static_cast<int>(_var_w / (aspect_ratio));
#define IMG_HEIGHT(_var_w, _var_h, aspect_ratio, height) const int _var_h = (height); const int _var_w = static_cast<int>(_var_h * (aspect_ratio));

#include "util/vec3.h"
#include "shader/ray_march_shader.h"
#include "render/renderer.h"


constexpr int WORKER_COUNT = 96;

int main() {
    // Image data
    IMG_HEIGHT(image_width, image_height, 16.0 / 9.0, 600)
    const int channels = 3;
    unsigned char img_data[image_width * image_height * channels];

    // Render thread function
    using shader_type = ray_march_shader;

    auto render_job = [](unsigned char* img_data, int start_index, int end_index) {
        return [img_data, start_index, end_index]() {
            renderer<shader_type> render;
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
        workers.push_back(std::thread(render_job(img_data, start, end)));
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
