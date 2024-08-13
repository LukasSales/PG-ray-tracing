#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material {
public:
    Vec3 kd;  // Coeficiente difuso
    Vec3 ks;  // Coeficiente especular
    Vec3 ka;  // Coeficiente ambiental
    Vec3 kr;  // Coeficiente de reflexão
    Vec3 kt;  // Coeficiente de transmissão
    float eta; // Coeficiente de rugosidade
    float refraction; // Índice de refração 5 entrega

    Material() 
        : kd(Vec3(0, 0, 0)), ks(Vec3(0, 0, 0)), ka(Vec3(0, 0, 0)), 
          kr(Vec3(0, 0, 0)), kt(Vec3(0, 0, 0)), eta(0), refraction(1.0) {}

    Material(const Vec3& kd, const Vec3& ks, const Vec3& ka, const Vec3& kr, const Vec3& kt, float eta, float refraction)
        : kd(kd), ks(ks), ka(ka), kr(kr), kt(kt), eta(eta), refraction(refraction) {}
};

#endif // MATERIAL_H
