#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "point3.h"

class Ray {
public:
    Point3 origin;
    Vec3 direction;

    Ray(const Point3& origin, const Vec3& direction)
        : origin(origin), direction(direction) {}

    Point3 at(double t) const {
        return origin + direction * t;
    }
};

#endif