#ifndef PLANE_H
#define PLANE_H

#include "point3.h"
#include "vec3.h"
#include "ray.h"
#include "intersection.h"

class Plane {
public:
    Point3 point;
    Vec3 normal;
    Vec3 color;

    Plane(Point3 p, Vec3 n, Vec3 col) : point(p), normal(n), color(col) {}

    bool intersect(const Ray& ray, Intersection& intersection) const {
        double denom = normal.dot(ray.direction);
        if (fabs(denom) > 1e-6) {
            Vec3 p0l0(point.x - ray.origin.x, point.y - ray.origin.y, point.z - ray.origin.z);
            double t = p0l0.dot(normal) / denom;
            if (t >= 0) {
                intersection = Intersection(t, color);
                return true;
            }
        }
        return false;
    }
};

#endif // PLANE_H
