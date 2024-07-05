#ifndef POINT3_H
#define POINT3_H

#include "vec3.h"

class Point3 {
public:
    double x, y, z;

    Point3() : x(0), y(0), z(0) {}
    Point3(double x, double y, double z) : x(x), y(y), z(z) {}

    Point3 operator+(const Vec3& v) const {
        return Point3(x + v.x, y + v.y, z + v.z);
    }

    Point3 operator-(const Vec3& v) const {
        return Point3(x - v.x, y - v.y, z - v.z);
    }

    Vec3 operator-(const Point3& p) const {
        return Vec3(x - p.x, y - p.y, z - p.z);
    }
};

#endif // POINT3_H
