#ifndef SPHERE_H
#define SPHERE_H

#include "point3.h"
#include "vec3.h"
#include "ray.h"
#include "intersection.h"

class Sphere {
public:
    Point3 center;
    double radius;
    Vec3 color;

    Sphere(Point3 c, double r, Vec3 col) : center(c), radius(r), color(col) {}

    bool intersect(const Ray& ray, Intersection& intersection) const {
        Vec3 oc(ray.origin.x - center.x, ray.origin.y - center.y, ray.origin.z - center.z);
        double a = ray.direction.dot(ray.direction);
        double b = 2.0 * oc.dot(ray.direction);
        double c = oc.dot(oc) - radius * radius;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0) return false;

        double t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t > 0) {
            intersection = Intersection(t, color);
            return true;
        }

        t = (-b + sqrt(discriminant)) / (2.0 * a);
        if (t > 0) {
            intersection = Intersection(t, color);
            return true;
        }

        return false;
    }
};

#endif // SPHERE_H
