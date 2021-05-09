#ifndef RAYTRACING_IN_A_WEEKEND_LIGHT_H
#define RAYTRACING_IN_A_WEEKEND_LIGHT_H

/**
 * Abstract light source class
 */
class light_source {
public:
    /**
     * Returns the direction from which light is coming from this source to the given destination in world space
     * @param p Light destination
     * @return Light direction at that point
     */
    virtual vec3 light_dir(const point3& p) const = 0;

    /**
     * Returns the intensity of the light at the given destination
     * @param p Light destination
     * @return Light intensity at that point
     */
    virtual double intensity(const point3& p) const = 0;

    virtual ~light_source() {}
};

/**
 * Global light source: light is always coming from the same direction and at constant intensity
 */
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

/**
 * Point light source: Light is emitted radially from a single point in all directions at constant intensity
 */
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
