#ifndef RAYTRACING_IN_A_WEEKEND_OBJECTS_H
#define RAYTRACING_IN_A_WEEKEND_OBJECTS_H

#include <vector>
#include "../../util/vec3.h"
#include "../../util/math.h"
#include "light.h"

class sdf_object {
public:
    sdf_object() : diffuse_color(color(1, 1, 1)) {}

    virtual double sdf(const vec3& p) const = 0;
    virtual vec3 normal(const vec3& p) const {
        static vec3 DX(NORMAL_STEP, 0, 0);
        static vec3 DY(0, NORMAL_STEP, 0);
        static vec3 DZ(0, 0, NORMAL_STEP);
        double x1 = sdf(p - DX);
        double x2 = sdf(p + DX);
        double y1 = sdf(p - DY);
        double y2 = sdf(p + DY);
        double z1 = sdf(p - DZ);
        double z2 = sdf(p + DZ);
        return unit_vector(vec3(x2 - x1, y2 - y1, z2 - z1));
    }

    virtual ~sdf_object() {}

public:
    color diffuse_color;

private:
    static constexpr double NORMAL_STEP = 0.0008;
};

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
};

class sdf_padded : public sdf_object {
public:
    sdf_padded(sdf_object* _obj, double _padding) : obj(_obj), padding(_padding) {}

    double sdf(const vec3 &p) const override {
        return obj->sdf(p) - padding;
    }

    ~sdf_padded() {
        delete obj;
    }
private:
    sdf_object* obj;
    double padding;
};

class sdf_diff : public sdf_object {
public:
    sdf_diff(sdf_object* _o1, sdf_object* _o2) : o1(_o1), o2(_o2) { }

    double sdf(const vec3& p) const override {
        return max(o1->sdf(p), -(o2->sdf(p)));
    }

    ~sdf_diff() {
        delete o1;
        delete o2;
    }
private:
    sdf_object *o1, *o2;
};

class sdf_union : public sdf_object {
public:
    sdf_union(sdf_object* _o1, sdf_object* _o2) : o1(_o1), o2(_o2) { }

    double sdf(const vec3& p) const override {
        return min(o1->sdf(p), o2->sdf(p));
    }

    ~sdf_union() {
        delete o1;
        delete o2;
    }
private:
    sdf_object *o1, *o2;
};

class sdf_sphere : public sdf_object {
public:
    sdf_sphere(vec3 _pos, double _radius) : pos(_pos), radius(_radius) {}

    double sdf(const vec3& p) const override {
        return (p - pos).length() - radius;
    }

    vec3 normal(const vec3& p) const override {
        return unit_vector(p - pos);
    }

private:
    vec3 pos;
    double radius;
};

class sdf_cylinder : public sdf_object {
public:
    sdf_cylinder(vec3 _pos, double _height, double _radius) : pos(_pos), height(_height), radius(_radius) {}

    double sdf(const vec3& p) const override {
        vec3 q = p - pos;

        double dxz = max(0.0, sqrt(q.x() * q.x() + q.z() * q.z()) - radius);
        double dy = max(0.0, abs(q.y()) - height / 2);

        return sqrt(dxz*dxz + dy*dy);
    }

    vec3 normal(const vec3& p) const override {
        if (p.y() > (pos.y() + height / 2)) return vec3(0, 1, 0);
        if (p.y() < (pos.y() - height / 2)) return vec3(0, -1, 0);
        return unit_vector(vec3(p.x() - pos.x(), 0, p.z() - pos.z()));
    }

private:
    vec3 pos;
    double height;
    double radius;
};

class sdf_ground_plane : public sdf_object {
public:
    explicit sdf_ground_plane(double _height) : height(_height) {}

    double sdf(const vec3& p) const override {
        return (p.y() - height);
    }

    vec3 normal(const vec3& p) const override {
        return vec3(0, 1, 0);
    }

private:
    double height;
};

#endif //RAYTRACING_IN_A_WEEKEND_OBJECTS_H
