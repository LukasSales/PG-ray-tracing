#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <vector>
#include "matrix4.h"
#include "sphere.h"
#include "plane.h"
#include "mesh.h"
#include "torus.h"

// Aplica a transformação a uma lista de esferas, planos, meshes e toros
void applyTransformation(const Matrix4& transform, std::vector<Sphere>& spheres, std::vector<Plane>& planes, std::vector<Mesh>& meshes, std::vector<Torus>& tori) {
    // Transformar esferas
    for (auto& sphere : spheres) {
        sphere.center = transform * sphere.center;  // Transforma o centro da esfera
    }
    
    // Transformar planos
    for (auto& plane : planes) {
        plane.point = transform * plane.point;  // Transforma um ponto no plano
        plane.normal = transform * plane.normal;  // Transforma a normal do plano
    }
    
    // Transformar meshes
    for (auto& mesh : meshes) {
        for (auto& vertex : mesh.Vertices) {
            vertex = transform * vertex;  // Transforma cada vértice do mesh
        }
    }
    
    // Transformar toros
    for (auto& torus : tori) {
        // Transformar os vértices do toro
        for (auto& vertex : torus.Vertices) {
            vertex = transform * vertex;  // Transforma cada vértice da malha do toro
        }

        // Transformar os triângulos do toro
        for (auto& triangle : torus.Triangles) {
            // Atualiza os vértices do triângulo com a transformação
            triangle.A = transform * triangle.A;  // Transforma o vértice A
            triangle.B = transform * triangle.B;  // Transforma o vértice B
            triangle.C = transform * triangle.C;  // Transforma o vértice C
        }
    }
}

#endif // TRANSFORMATION_H
