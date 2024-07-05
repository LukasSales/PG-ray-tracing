#ifndef MESH_H
#define MESH_H

#include <vector>
#include <limits>
#include <cmath>
#include "vec3.h"
#include "point3.h"
#include "ray.h"
#include "plane.h"
#include "intersection.h"

class Triangle {
public:
    Point3 A, B, C;

    Triangle(Point3 A, Point3 B, Point3 C) : A(A), B(B), C(C) {}

    // Função para calcular a área de um triângulo usando o produto vetorial
    double Area(Point3 X, Point3 Y, Point3 Z) const {
        Vec3 side1 = Y - X;
        Vec3 side2 = Z - X;
        return (side1.cross(side2)).norm() / 2;
    }

    // Função para verificar interseção do triângulo com um raio
    double intersect(const Ray& r) const {
        Vec3 normal = (B - A).cross(C - A);
        Plane plane(A, normal, Vec3(0, 0, 0)); // Corrigir o construtor para incluir a cor
        Intersection intersection;

        if (!plane.intersect(r, intersection)) return 0;

        Point3 P = r.at(intersection.distance); // Usando a distância do Intersection
        double alpha = Area(P, B, C) / Area(A, B, C);
        double beta = Area(P, A, C) / Area(A, B, C);
        double gamma = Area(P, A, B) / Area(A, B, C);

        bool hit = (alpha >= 0) && (beta >= 0) && (gamma >= 0) && (fabs(alpha + beta + gamma - 1) < 1e-6);
        return hit ? intersection.distance : 0; // Usando a distância do Intersection
    }
};

class Mesh {
public:
    std::vector<Point3> Vertices;
    std::vector<Triangle> Triangles;
    Vec3 color;

    Mesh(const std::vector<Point3>& vertices, const std::vector<std::vector<int>>& triplas, const Vec3& color) 
        : Vertices(vertices), color(color) {
        for (const auto& tripla : triplas) {
            Triangles.emplace_back(vertices[tripla[0]], vertices[tripla[1]], vertices[tripla[2]]);
        }
    }

    // Função para verificar interseção da malha com um raio
    double intersect(const Ray& r) const {
        double menor_dist = std::numeric_limits<double>::max();
        for (const auto& tri : Triangles) {
            double dist = tri.intersect(r);
            if (dist > 0 && dist < menor_dist) {
                menor_dist = dist;
            }
        }
        return (menor_dist == std::numeric_limits<double>::max()) ? 0 : menor_dist;
    }
};

#endif // MESH_H
