#include "ray_march_shader.h"

#include <vector>

using std::vector;

constexpr double MAX_DIST = 30;
constexpr double MIN_STEP = 0.0001;
constexpr double DIST_THRESHOLD = 0.00005;

raycast_info ray_march_shader::raycast(const ray& r, sdf_object* ignore, double min_travel) const {
    raycast_info info {r.at(MAX_DIST), nullptr, MAX_DIST};
    double t = min_travel;
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
            if (d < DIST_THRESHOLD) {
                info.target = obj;
                return info;
            }
            if (d < local_min_dist) local_min_dist = d;
        }
        t += max(MIN_STEP, local_min_dist);
    }
    return info;
}

ray_march_shader::ray_march_shader() {
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

color ray_march_shader::frag(const vec3 &uv) {
    color col = clear_color(uv);
    ray r = cam.get_ray(uv, true);

    double ambient_light = 0.15;
    double soft_shadow_falloff = 0.15;

    // Hitting an object?
    auto rr = raycast(r);
    if (rr.target != nullptr) {
        vec3 target_normal = rr.target->normal(rr.hitpoint);

        // Shadows & Lights
        double light = 0;
        for (auto light_src : scn.light_sources) {
            vec3 light_dir = light_src->light_dir(rr.hitpoint);

            ray shadow_ray(rr.hitpoint + 2 * MIN_STEP * target_normal, -light_dir);
            auto shadow_rr = raycast(shadow_ray);
            if (shadow_rr.target == nullptr) {
                // Diffuse light intensity
                double l = dot(target_normal, -light_dir) * light_src->intensity(rr.hitpoint);

                // Soft shadow
//                auto soft_shadow_rr = raycast(shadow_ray, nullptr, 0);
//                if (soft_shadow_rr.target == nullptr) {
//                    double soft_shadow_factor =
//                            smoothstep((soft_shadow_rr.min_dist - DIST_THRESHOLD) /
//                                       (soft_shadow_falloff - DIST_THRESHOLD));
//                    l *= soft_shadow_factor;
//                }

                light += max(l, 0.0);
            }
        }

        light = clamp(light, ambient_light, 1.0);
        col = light * rr.target->get_diffuse_color(rr.hitpoint);
    }

    return col;
}
