#ifndef POINT3_H
#define POINT3_H

class Point3 {
public:
    double x, y, z;
    
    Point3() : x(0), y(0), z(0) {}
    Point3(double x, double y, double z) : x(x), y(y), z(z) {}

    Point3 operator+(const Point3& p) const {
        return Point3(x + p.x, y + p.y, z + p.z);
    }

    Point3 operator-(const Point3& p) const {
        return Point3(x - p.x, y - p.y, z - p.z);
    }

    Point3 operator*(double t) const {
        return Point3(x * t, y * t, z * t);
    }
};

#endif // POINT3_H
