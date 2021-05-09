cmake_minimum_required(VERSION 3.19)
project(raytracing_in_a_weekend)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracing_in_a_weekend src/main.cpp src/util/vec3.cpp src/util/vec3.h src/shader/raymarch/ray.h src/shader/raymarch/camera.h src/shader/frag_shader.h src/shader/ray_march_shader.cpp src/shader/ray_march_shader.h src/util/math.h src/render/renderer.h src/shader/raymarch/objects.h src/shader/raymarch/light.h)