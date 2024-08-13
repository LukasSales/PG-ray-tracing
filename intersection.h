#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "vec3.h"
#include "point3.h"
#include "material.h"

class Intersection {
public:
    double distance;  // Distância da origem do raio até o ponto de interseção
    Vec3 normal;      // Normal no ponto de interseção
    Material material; // Material no ponto de interseção

    Intersection() : distance(std::numeric_limits<double>::max()), normal(Vec3()), material(Material()) {}
    Intersection(double distance, const Vec3& normal, const Material& material) 
        : distance(distance), normal(normal), material(material) {}
};

#endif // INTERSECTION_H
