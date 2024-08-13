#ifndef PLANE_H
#define PLANE_H

#include "point3.h"
#include "vec3.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"

class Plane {
public:
    Point3 point;
    Vec3 normal;
    Material material;

    Plane(Point3 p, Vec3 n, const Material& material)
        : point(p), normal(n.normalize()), material(material) {}

    bool intersect(const Ray& ray, Intersection& intersection) const {
        double denom = normal.dot(ray.direction);
        if (fabs(denom) > 1e-6) {
            double t = (point - ray.origin).dot(normal) / denom;
            if (t >= 0) {
                intersection = Intersection(t, normal, material);
                return true;
            }
        }
        return false;
    }
};

#endif // PLANE_H
