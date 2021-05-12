//
// Created by david on 12.05.2021.
//

#include "ray_march_edge_detection_shader.h"

void ray_march_edge_detection_shader::frag_ray(raycast_info r_info, color& out_col) {
    // Ray cast closely passing an object -> edge
    if (r_info.target == nullptr && r_info.min_dist < 0.02) {
        out_col = color(0.1);
    }
}