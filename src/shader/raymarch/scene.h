#ifndef CPU_RAYMARCHER_SCENE_H
#define CPU_RAYMARCHER_SCENE_H

#include "objects.h"
#include "light.h"

/**
 * Container object for sdf objects and light sources
 */
class scene {
public:
    scene& operator+=(sdf_object* obj) {
        objects.push_back(obj);
        return *this;
    }

    scene& operator+=(light_source* obj) {
        light_sources.push_back(obj);
        return *this;
    }

    ~scene() {
        for(auto obj : objects) delete obj;
        for(auto lsrc : light_sources) delete lsrc;
    }
public:
    std::vector<sdf_object*> objects;
    std::vector<light_source*> light_sources;
    double ambient_light;
};

#endif //CPU_RAYMARCHER_SCENE_H
