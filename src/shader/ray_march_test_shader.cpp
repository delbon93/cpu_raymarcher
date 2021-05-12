#include "ray_march_test_shader.h"

void ray_march_test_shader::frag_ray(raycast_info r_info, color& out_col) {
    if (r_info.target != nullptr) {
        vec3 target_normal = r_info.target->normal(r_info.hitpoint);

        // Shadows & Lights
        double light = 0;
        for (auto light_src : scn.light_sources) {
            vec3 light_dir = light_src->light_dir(r_info.hitpoint);

            ray shadow_ray(r_info.hitpoint + 2 * MIN_STEP * target_normal, -light_dir);
            auto shadow_rr = raycast(shadow_ray, distThreshold);
            if (shadow_rr.target == nullptr) {
                // Diffuse light intensity
                double l = dot(target_normal, -light_dir) * light_src->intensity(r_info.hitpoint);

                light += max(l, 0.0);
            }
        }

        light = clamp(light, scn.ambient_light, 1.0);
        out_col = light * r_info.target->get_diffuse_color(r_info.hitpoint);
    }
}
