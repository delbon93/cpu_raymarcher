#ifndef RAYTRACING_IN_A_WEEKEND_LIGHT_H
#define RAYTRACING_IN_A_WEEKEND_LIGHT_H

class light_source {
public:
    light_source() : clr(color(1, 1, 1)) {}

    virtual vec3 light_dir(const point3& p) const = 0;
    virtual double intensity(const point3& p) const = 0;
    virtual color light_clr(const point3& p) const { return clr; }

    virtual ~light_source() {}
private:
    color clr;
};

class global_light_source : public light_source {
public:
    global_light_source(vec3 _light_dir, double _intensity) : dir(_light_dir), intnsty(_intensity) {}

    vec3 light_dir(const point3& p) const override {
        return dir;
    }

    double intensity(const point3& p) const override {
        return intnsty;
    }

private:
    vec3 dir;
    double intnsty;

};

class point_light_source : public light_source {
public:
    point_light_source(point3 _pos, double _intensity) : pos(_pos), intnsty(_intensity) {}

    inline double dist(const point3& p) { return (p - pos).length(); }

    vec3 light_dir(const point3& p) const override {
        return unit_vector(p - pos);
    }

    double intensity(const point3& p) const override {
        return intnsty;
    }

private:
    point3 pos;
    double intnsty;
};

#endif //RAYTRACING_IN_A_WEEKEND_LIGHT_H
