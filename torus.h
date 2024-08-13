#ifndef TORUS_H
#define TORUS_H

#include "mesh.h"
#include <cmath>
#include <vector>

class Torus : public Mesh {
public:
    Torus(const Point3& center, float majorRadius, float minorRadius, int numSides, int numRings, const Material& material) {
        generateTorus(center, majorRadius, minorRadius, numSides, numRings, material);
    }

private:
    void generateTorus(const Point3& center, float R, float r, int numSides, int numRings, const Material& material) {
        std::vector<Point3> vertices;
        std::vector<std::vector<int>> triplas;

        vertices.reserve(numSides * numRings); // Reservando espaço para os vértices
        triplas.reserve(numSides * numRings * 2); // Reservando espaço para as triplas

        for (int i = 0; i < numSides; ++i) {
            float theta = (float)i * 2.0f * M_PI / (float)numSides;
            float cosTheta = cosf(theta);
            float sinTheta = sinf(theta);
            for (int j = 0; j < numRings; ++j) {
                float phi = (float)j * 2.0f * M_PI / (float)numRings;
                float cosPhi = cosf(phi);
                float sinPhi = sinf(phi);
                float x = (R + r * cosPhi) * cosTheta;
                float y = (R + r * cosPhi) * sinTheta;
                float z = r * sinPhi;
                vertices.emplace_back(center.x + x, center.y + y, center.z + z);
            }
        }

        for (int i = 0; i < numSides; ++i) {
            for (int j = 0; j < numRings; ++j) {
                int next_i = (i + 1) % numSides;
                int next_j = (j + 1) % numRings;

                int index1 = i * numRings + j;
                int index2 = next_i * numRings + j;
                int index3 = i * numRings + next_j;
                int index4 = next_i * numRings + next_j;

                triplas.push_back({index1, index2, index3});
                triplas.push_back({index2, index4, index3});
            }
        }

        this->Vertices = vertices;
        this->Triangles.clear();
        this->Triangles.reserve(triplas.size());
        for (const auto& tripla : triplas) {
            this->Triangles.emplace_back(vertices[tripla[0]], vertices[tripla[1]], vertices[tripla[2]], material);
        }
        this->material = material;
    }
};

#endif // TORUS_H
