#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <vector>
#include "matrix4.h"
#include "sphere.h"
#include "plane.h"
#include "mesh.h"

// Aplica a transformação a uma lista de esferas, planos e meshes
void applyTransformation(const Matrix4& transform, std::vector<Sphere>& spheres, std::vector<Plane>& planes, std::vector<Mesh>& meshes) {
    for (auto& sphere : spheres) {
        sphere.center = transform * sphere.center;  // Transforma o centro da esfera
    }
    for (auto& plane : planes) {
        plane.point = transform * plane.point;  // Transforma um ponto no plano
        plane.normal = transform * plane.normal;  // Transforma a normal do plano
    }
    for (auto& mesh : meshes) {
        for (auto& vertex : mesh.Vertices) {
            vertex = transform * vertex;  // Transforma cada vértice do mesh
        }
    }
}

#endif
