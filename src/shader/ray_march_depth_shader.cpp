//
// Created by david on 12.05.2021.
//

#include "ray_march_depth_shader.h"

void ray_march_depth_shader::frag_ray(raycast_info r_info, color& out_col) {
    out_col = color(1.0 - smoothstep(r_info.travel / MAX_DIST)) * 0.85;
}