#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "vec3.h"

class Intersection {
public:
    double distance;
    Vec3 color;

    // Construtor padrão
    Intersection() : distance(0), color(Vec3(0, 0, 0)) {}

    // Construtor que inicializa distancia e color
    Intersection(double d, Vec3 c) : distance(d), color(c) {}
};

#endif // INTERSECTION_H
