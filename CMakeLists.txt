cmake_minimum_required(VERSION 3.19)
project(cpu_raymarcher)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpu_raymarcher src/main.cpp src/util/vec3.cpp src/util/vec3.h src/shader/raymarch/ray.h src/shader/raymarch/camera.h src/shader/frag_shader.h src/shader/ray_march_shader.cpp src/shader/ray_march_shader.h src/util/math.h src/render/renderer.h src/shader/raymarch/objects.h src/shader/raymarch/light.h src/shader/ray_march_test_shader.cpp src/shader/ray_march_test_shader.h src/shader/raymarch/scene.h src/shader/ray_march_depth_shader.cpp src/shader/ray_march_depth_shader.h)