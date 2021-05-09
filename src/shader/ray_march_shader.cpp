#include "ray_march_shader.h"

#include <vector>

using std::vector;

constexpr double MAX_DIST = 30;
constexpr double MIN_STEP = 0.0001;
constexpr double DIST_THRESHOLD = 0.00005;

raycast_info ray_march_shader::raycast(const ray& r, sdf_object* ignore) const {
    raycast_info info {r.at(MAX_DIST), nullptr, MAX_DIST};
    double t = 0.0;
    while (t < MAX_DIST) {
        double local_min_dist = MAX_DIST;
        vec3 p = r.at(t);
        for(auto obj : scn.objects) {
            if (obj == ignore) continue;
            double d = obj->sdf(p);
            if (d < info.min_dist) {
                info.min_dist = d;
                info.hitpoint = p;
            }
            if (d < local_min_dist) local_min_dist = d;
            if (d < DIST_THRESHOLD) {
                info.target = obj;
                return info;
            }
        }
        t += max(MIN_STEP, local_min_dist);
    }
    return info;
}

ray_march_shader::ray_march_shader() {
    auto ball_with_hole = new sdf_padded(new sdf_diff(
        new sdf_sphere(point3(0, 0, -1.5), .3),
        new sdf_sphere(point3(-0.13, 0.2, -1.4), .4)
    ), 0.1);
    ball_with_hole->diffuse_color = color(0.7, 0.3, 0.4);
    scn += ball_with_hole;

    auto small_ball = new sdf_sphere(point3(-0.6, 0.6, -1.22), .15);
    small_ball->diffuse_color = color(0.1, 0.3, 0.8);
    scn += small_ball;

    auto ground = new sdf_ground_plane(-1.0);
    ground->diffuse_color = color(0.15, 0.75, 0.3);
    scn += ground;

    auto cylinder = new sdf_cylinder(vec3(-2, -0.7, -4), 0.6, 0.2);
    cylinder->diffuse_color = color(0.7, 0.65, 0.3);
    scn += cylinder;

    scn += new global_light_source(unit_vector(vec3(1, -0.4, -1.5)), 0.8);
    scn += new point_light_source(vec3(0, 2, -0.4), 0.4);
}

color ray_march_shader::frag(const vec3 &uv) {
    color col = clear_color(uv);
    ray r = cam.get_ray(uv, true);

    double ambient_light = 0.15;
    double soft_shadow_falloff = 0.15;

    // Hitting an object?
    auto rr = raycast(r, nullptr);
    if (rr.target != nullptr) {
        double light = 0;

        // Shadows & Lights
        for (auto light_src : scn.light_sources) {
            vec3 light_dir = light_src->light_dir(rr.hitpoint);

            ray shadow_ray(rr.hitpoint, -light_dir);
            auto shadow_rr = raycast(shadow_ray, rr.target);
            if (shadow_rr.target == nullptr) {
                // core shadow
                double l = dot(rr.target->normal(rr.hitpoint), -light_dir) * light_src->intensity(rr.hitpoint);

                // soft shadow
                double soft_shadow_factor =
                        smoothstep((shadow_rr.min_dist - DIST_THRESHOLD) / (soft_shadow_falloff - DIST_THRESHOLD));
                l *= soft_shadow_factor;
                light += max(l, 0.0);
            }
        }

        light = clamp(light, ambient_light, 1.0);
        col = light * rr.target->diffuse_color;
    }

    return col;
}
