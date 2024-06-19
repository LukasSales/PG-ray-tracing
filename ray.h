#ifndef RAY_H
#define RAY_H

#include "point3.h"
#include "vec3.h"

class Ray {
public:
    Point3 origin;
    Vec3 direction;

    Ray(Point3 o, Vec3 d) : origin(o), direction(d) {}
};

#endif // RAY_H
