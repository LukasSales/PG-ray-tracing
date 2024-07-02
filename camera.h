#ifndef CAMERA_H
#define CAMERA_H

#include "point3.h"
#include "vec3.h"
#include "ray.h"

class Camera {
public:
    Point3 position;
    Point3 lookAt;
    Vec3 up;
    double distance;
    int vres, hres;

    Camera(Point3 pos, Point3 look, Vec3 upVec, double dist, int vertRes, int horizRes)
        : position(pos), lookAt(look), up(upVec), distance(dist), vres(vertRes), hres(horizRes) {}

    void getCameraBasis(Vec3 &u, Vec3 &v, Vec3 &w) const {
        w = Vec3(position.x - lookAt.x, position.y - lookAt.y, position.z - lookAt.z).normalize();
        u = up.cross(w).normalize();
        v = w.cross(u);
    }

    Ray getRay(int x, int y) const {
        Vec3 u, v, w;
        getCameraBasis(u, v, w);
        double aspect_ratio = double(hres) / double(vres);
        double u_coord = (2 * ((x + 0.5) / hres) - 1) * aspect_ratio;
        double v_coord = 1 - 2 * ((y + 0.5) / vres);
        Vec3 direction = (u_coord * u + v_coord * v - distance * w).normalize();
        return Ray(position, direction);
    }
};

#endif // CAMERA_H
