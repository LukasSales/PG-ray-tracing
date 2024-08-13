#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"
#include "point3.h"

class Light {
public:
    Point3 position;
    Vec3 intensity;

    Light(const Point3& position, const Vec3& intensity)
        : position(position), intensity(intensity) {}
};

#endif // LIGHT_H
