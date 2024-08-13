#ifndef MESH_H
#define MESH_H
#define M_PI 3.14159265358979323846

#include <vector>
#include <limits>
#include <cmath>
#include "vec3.h"
#include "point3.h"
#include "ray.h"
#include "plane.h"
#include "intersection.h"
#include "material.h"

class Triangle {
public:
    Point3 A, B, C;
    Material material;

    Triangle(Point3 A, Point3 B, Point3 C, const Material& material) 
        : A(A), B(B), C(C), material(material) {}

    // Função para calcular a área de um triângulo usando o produto vetorial
    double Area(Point3 X, Point3 Y, Point3 Z) const {
        Vec3 side1 = Y - X;
        Vec3 side2 = Z - X;
        return (side1.cross(side2)).norm() / 2;
    }

    // Função para verificar interseção do triângulo com um raio
    bool intersect(const Ray& r, Intersection& intersection) const {
        Vec3 normal = (B - A).cross(C - A).normalize();
        Plane plane(A, normal, material);
        if (!plane.intersect(r, intersection)) return false;

        Point3 P = r.at(intersection.distance);
        double alpha = Area(P, B, C) / Area(A, B, C);
        double beta = Area(P, A, C) / Area(A, B, C);
        double gamma = Area(P, A, B) / Area(A, B, C);

        bool hit = (alpha >= 0) && (beta >= 0) && (gamma >= 0) && (fabs(alpha + beta + gamma - 1) < 1e-6);
        if (hit) {
            intersection.distance = intersection.distance;
            intersection.material = material;
            intersection.normal = normal;
            return true;
        }
        return false;
    }
};

class Mesh {
public:
    std::vector<Point3> Vertices;
    std::vector<Triangle> Triangles;
    Material material;

    // Construtor padrão
    Mesh() = default;

    // Construtor com vértices e triângulos
    Mesh(const std::vector<Point3>& vertices, const std::vector<std::vector<int>>& triplas, const Material& material) {
        for (const auto& tripla : triplas) {
            Triangles.emplace_back(vertices[tripla[0]], vertices[tripla[1]], vertices[tripla[2]], material);
        }
    }

    // Construtor para criar um toro
    Mesh(const Point3& center, double minor_radius, double major_radius, size_t num_circles, size_t verts_per_circle, const Material& mat) {
        std::vector<Point3> vertices;
        std::vector<std::vector<int>> triplas;

        for (size_t i = 0; i < num_circles; ++i) {
            double theta = (i * 2.0 * M_PI) / num_circles;
            for (size_t j = 0; j < verts_per_circle; ++j) {
                double phi = (j * 2.0 * M_PI) / verts_per_circle;
                double x = (major_radius + minor_radius * cos(phi)) * cos(theta);
                double y = (major_radius + minor_radius * cos(phi)) * sin(theta);
                double z = minor_radius * sin(phi);
                vertices.emplace_back(center.x + x, center.y + y, center.z + z);
            }
        }

        for (size_t i = 0; i < num_circles; ++i) {
            for (size_t j = 0; j < verts_per_circle; ++j) {
                int next_i = (i + 1) % num_circles;
                int next_j = (j + 1) % verts_per_circle;

                int index1 = i * verts_per_circle + j;
                int index2 = next_i * verts_per_circle + j;
                int index3 = i * verts_per_circle + next_j;
                int index4 = next_i * verts_per_circle + next_j;

                triplas.push_back({index1, index2, index3});
                triplas.push_back({index2, index4, index3});
            }
        }

        for (const auto& tripla : triplas) {
            Triangles.emplace_back(vertices[tripla[0]], vertices[tripla[1]], vertices[tripla[2]], mat);
        }

        Vertices = vertices;
        material = mat;
    }

    // Função para verificar interseção da malha com um raio
    bool intersect(const Ray& r, Intersection& intersection) const {
        double menor_dist = std::numeric_limits<double>::max();
        bool hit = false;
        for (const auto& tri : Triangles) {
            Intersection temp_intersection;
            if (tri.intersect(r, temp_intersection) && temp_intersection.distance < menor_dist) {
                menor_dist = temp_intersection.distance;
                intersection = temp_intersection;
                hit = true;
            }
        }
        return hit;
    }

    // Função para calcular a normal da malha no ponto dado
    Vec3 calculateNormal(const Point3& point) const {
        Vec3 normalSum(0, 0, 0);
        int count = 0;

        for (const auto& tri : Triangles) {
            Vec3 normal = (tri.B - tri.A).cross(tri.C - tri.A).normalize();
            if (point == tri.A || point == tri.B || point == tri.C) {
                normalSum = normalSum + normal;
                count++;
            }
        }

        if (count > 0) {
            return normalSum / count;
        } else {
            return Vec3(0, 0, 1); // Normal padrão
        }
    }
};

#endif // MESH_H
