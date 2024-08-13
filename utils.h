#pragma once

#include <algorithm>
#include "vec3.h"

// Função clamp para limitar um valor dentro de um intervalo
template <typename T>
T clamp(T val, T min, T max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

// Função para calcular a reflexão de um vetor
Vec3 reflect(const Vec3& incident, const Vec3& normal) {
    return incident - 2 * normal.dot(incident) * normal;
}

// Função para calcular a refração de um vetor
Vec3 refract(const Vec3& incident, const Vec3& normal, double ior) {
    double cosi = clamp(-1.0, 1.0, incident.dot(normal));
    double etai = 1, etat = ior;
    Vec3 n = normal;
    if (cosi < 0) {
        cosi = -cosi;
    } else {
        std::swap(etai, etat);
        n = -normal;
    }
    double eta = etai / etat;
    double k = 1 - eta * eta * (1 - cosi * cosi);
    if (k < 0) {
        return Vec3(0, 0, 0);
    } else {
        return eta * incident + (eta * cosi - std::sqrt(k)) * n;
    }
}
