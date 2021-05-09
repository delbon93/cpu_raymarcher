#ifndef RAYTRACING_IN_A_WEEKEND_VEC3_H
#define RAYTRACING_IN_A_WEEKEND_VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

/**
 * 3-component vector class
 */
class vec3 {
public:
    /**
     * Creates zero-vector
     */
    vec3() : e{0, 0, 0} {}

    /**
     * Creates vector with all components set to the same value
     * @param e0 Value of all components
     */
    vec3(double e0) : e{e0, e0, e0} {}

    /**
     * Creates vector with given components
     * @param e0 First component
     * @param e1 Second component
     * @param e2 Third component
     */
    vec3(double e0, double e1, double e2) : e {e0, e1, e2} {}

    /**
     * Spatial alias for first component
     * @return First component
     */
    double x() const { return e[0]; }

    /**
     * Spatial alias for second component
     * @return Second component
     */
    double y() const { return e[1]; }

    /**
     * Spatial alias for third component
     * @return Third component
     */
    double z() const { return e[2]; }

    /**
     * Color alias for first component
     * @return First component
     */
    double r() const { return e[0]; }

    /**
     * Color alias for second component
     * @return Second component
     */
    double g() const { return e[1]; }

    /**
     * Color alias for third component
     * @return Third component
     */
    double b() const { return e[2]; }

    vec3 operator-() const;
    double operator[](int i) const;
    double& operator[](int i);

    vec3& operator+=(const vec3 &v);
    vec3& operator*=(double t);
    vec3& operator/=(double t);

    /**
     * @return Magnitude of the vector
     */
    double length() const;

    /**
     * @return Squared magnitude of the vector
     */
    double length_squared() const;
private:
    double e[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.x() << " " << v.y() << " " << v.z();
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.x(), t*v.y(), t*v.z());
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

/**
 * Calculates the dot product of two vectors
 * @param u Vector
 * @param v Vector
 * @return Dot product of u and v
 */
inline double dot(const vec3 &u, const vec3 &v) {
    return u.x() * v.x()
           + u.y() * v.y()
           + u.z() * v.z();
}

/**
 * Calculates the cross product of two vectors
 * @param u Vector
 * @param v Vector
 * @return Cross product of u and v
 */
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.y() * v.z() - u.z() * v.y(),
                u.z() * v.x() - u.x() * v.z(),
                u.x() * v.y() - u.y() * v.x());
}

/**
 * Returns a unit vector that points into the direction of v
 * @param v Vector
 * @return Unit vector of v
 */
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

/**
 * Returns a unit vector constructed from given vector components
 * @param e0 First component
 * @param e1 Second component
 * @param e2 Third component
 * @return Unit vector
 */
inline vec3 unit_vector(double e0, double e1, double e2) {
    return unit_vector(vec3(e0, e1, e2));
}

#endif //RAYTRACING_IN_A_WEEKEND_VEC3_H
