#ifndef CAMERA_H
#define CAMERA_H

#include "point3.h"
#include "vec3.h"

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
};

#endif // CAMERA_H
