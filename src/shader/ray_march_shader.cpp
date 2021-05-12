#include "ray_march_shader.h"

raycast_info ray_march_shader::raycast(const ray& r, double distance_threshold, sdf_object* ignore, double min_travel) const {
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
            if (d < distance_threshold) {
                info.target = obj;
                return info;
            }
            if (d < local_min_dist) local_min_dist = d;
        }
        t += max(MIN_STEP, local_min_dist);
    }
    return info;
}

color ray_march_shader::frag(const vec3 &uv) {
    color col = clear_color(uv);
    ray r = cam.get_ray(uv, true);

    frag_ray(raycast(r, distThreshold), col);

    return col;
}
