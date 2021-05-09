#ifndef RAYTRACING_IN_A_WEEKEND_OBJECTS_H
#define RAYTRACING_IN_A_WEEKEND_OBJECTS_H

#include <vector>
#include "../../util/vec3.h"
#include "../../util/math.h"
#include "light.h"

/**
 * Abstract generic 'signed distance function'-object.
 */
class sdf_object {
public:
    sdf_object() : diffuse_color(color(1, 1, 1)), position(vec3()) {}

    /**
     * Calculates the signed distance from a given point in world space to the object.
     * Evaluates to exactly 0 at the objects surfaces, and positive distances to the closest
     * point on the surface elsewhere
     * @param p Point in world space
     * @return Shortest distance from that point to the object's surface
     */
    virtual double sdf(const vec3& p) const = 0;

    /**
     * Calculates the normal unit vector of the object's surface for a given point. This point does
     * not necessarily have to be on the surface exactly.
     * This default implementation approximates the normal as the gradient of the signed distance field. Specific,
     * more performant or accurate implementations for specific shapes can be provided by overriding this function.
     * @param p Point in world space
     * @return Unit vector that points from the object's surface to p or into the normal direction of the
     * surface if p is on the object's surface
     */
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

    virtual vec3 get_pos() const { return position; }
    void set_pos(point3 p) { position = p; }
    virtual color get_diffuse_color(point3& p) const { return diffuse_color; }
    void set_diffuse_color(color c) { diffuse_color = c; }

    virtual ~sdf_object() {}

private:
    color diffuse_color;
    point3 position;

private:
    static constexpr double NORMAL_STEP = 0.0008;
};

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
};

/************************
 *   SDF Object Types   *
 ************************/

class sdf_padded : public sdf_object {
public:
    sdf_padded(sdf_object* _obj, double _padding) : obj(_obj), padding(_padding) {}

    double sdf(const vec3 &p) const override {
        return obj->sdf(p) - padding;
    }

    vec3 get_pos() const override {
        return obj->get_pos();
    }

    ~sdf_padded() {
        delete obj;
    }
private:
    sdf_object* obj;
    double padding;
};

class sdf_composite : public sdf_object {
public:
    sdf_composite(point3 _position, sdf_object* _o1, sdf_object* _o2) : o1(_o1), o2(_o2) {
        set_pos(_position);
    }

    color get_diffuse_color(point3& p) const override {
        if (o1->sdf(p-get_pos()) < o2->sdf(p-get_pos())) return o1->get_diffuse_color(p);
        else return o2->get_diffuse_color(p);
    }

    ~sdf_composite() {
        delete o1;
        delete o2;
    }

protected:
    sdf_object *o1, *o2;
};

class sdf_diff : public sdf_composite {
public:
    sdf_diff(const point3 &position, sdf_object *o1, sdf_object *o2) : sdf_composite(position, o1, o2) {}

    double sdf(const vec3& p) const override {
        return max(o1->sdf(p-get_pos()), -(o2->sdf(p-get_pos())));
    }
};

class sdf_union : public sdf_composite {
public:
    sdf_union(const point3 &position, sdf_object *o1, sdf_object *o2) : sdf_composite(position, o1, o2) {}

    double sdf(const vec3& p) const override {
        return min(o1->sdf(p-get_pos()), o2->sdf(p-get_pos()));
    }
};

class sdf_intersect : public sdf_composite {
public:
    sdf_intersect(const point3 &position, sdf_object *o1, sdf_object *o2) : sdf_composite(position, o1, o2) {}

    double sdf(const vec3& p) const override {
        return max(o1->sdf(p-get_pos()), o2->sdf(p-get_pos()));
    }
};

class sdf_sphere : public sdf_object {
public:
    sdf_sphere(vec3 _pos, double _radius) : radius(_radius) {
        set_pos(_pos);
    }

    double sdf(const vec3& p) const override {
        return (p - get_pos()).length() - radius;
    }

    vec3 normal(const vec3& p) const override {
        return unit_vector(p - get_pos());
    }

private:
    double radius;
};

class sdf_cylinder : public sdf_object {
public:
    sdf_cylinder(vec3 _pos, double _height, double _radius) : height(_height), radius(_radius) {
        set_pos(_pos);
    }

    double sdf(const vec3& p) const override {
        vec3 q = p - get_pos();

        double dxz = max(0.0, sqrt(q.x() * q.x() + q.z() * q.z()) - radius);
        double dy = max(0.0, abs(q.y()) - height / 2);

        return sqrt(dxz*dxz + dy*dy);
    }

    vec3 normal(const vec3& p) const override {
        if (p.y() > (get_pos().y() + height / 2)) return vec3(0, 1, 0);
        if (p.y() < (get_pos().y() - height / 2)) return vec3(0, -1, 0);
        return unit_vector(vec3(p.x() - get_pos().x(), 0, p.z() - get_pos().z()));
    }

private:
    double height;
    double radius;
};

class sdf_capsule : public sdf_object {
public:
    sdf_capsule(vec3 _p1, vec3 _p2, double _radius) : p2(_p2), radius(_radius) {
        set_pos(_p1);
        length = (_p2 - _p1).length();
        v = unit_vector(_p2 - _p1);
    }

    sdf_capsule(vec3 _p, vec3 _dir, double _length, double _radius) : p2(_p+_dir*_length),
        length(_length), v(_dir), radius(_radius) {
        set_pos(_p);
    }

    double sdf(const vec3 &p) const override {
        double lambda = clamp(dot(p - get_pos(), v), 0.0, length);
        return ((get_pos() + lambda * v) - p).length() - radius;
    }

    vec3 normal(const vec3 &p) const override {
        double lambda = clamp(dot(p - get_pos(), v), 0.0, length);
        return unit_vector(p - (get_pos() + lambda * v));
    }

private:
    vec3 p2;
    vec3 v;
    double length;
    double radius;
};

class sdf_ground_plane : public sdf_object {
public:
    explicit sdf_ground_plane(double _height) {
        set_pos(vec3(0, _height, 0));
    }

    double sdf(const vec3& p) const override {
        return (p.y() - get_pos().y());
    }

    vec3 normal(const vec3& p) const override {
        return vec3(0, 1, 0);
    }
};

#endif //RAYTRACING_IN_A_WEEKEND_OBJECTS_H
