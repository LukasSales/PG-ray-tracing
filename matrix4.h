#ifndef MATRIX4_H
#define MATRIX4_H

#include "vec3.h"  // Inclui a definição da classe Vec3
#include "point3.h"  // Inclui a definição da classe Point3
#include <cmath>  // Inclui funções matemáticas como cos e sin

class Matrix4 {
public:
    double m[4][4];  // Matriz 4x4 de doubles

    // Construtor que inicializa a matriz identidade
    Matrix4() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = (i == j) ? 1.0 : 0.0;  // Define matriz identidade
            }
        }
    }

    // matriz de translação
    static Matrix4 translation(double tx, double ty, double tz) {
        Matrix4 result;
        result.m[0][3] = tx;
        result.m[1][3] = ty;
        result.m[2][3] = tz;
        return result;
    }

    // matriz de escala
    static Matrix4 scaling(double sx, double sy, double sz) {
        Matrix4 result;
        result.m[0][0] = sx;
        result.m[1][1] = sy;
        result.m[2][2] = sz;
        return result;
    }

    // matriz de rotação ao longo do eixo X
    static Matrix4 rotationX(double angle) {
        Matrix4 result;
        result.m[1][1] = cos(angle);
        result.m[1][2] = -sin(angle);
        result.m[2][1] = sin(angle);
        result.m[2][2] = cos(angle);
        return result;
    }

    // matriz de rotação ao longo do eixo Y
    static Matrix4 rotationY(double angle) {
        Matrix4 result;
        result.m[0][0] = cos(angle);
        result.m[0][2] = sin(angle);
        result.m[2][0] = -sin(angle);
        result.m[2][2] = cos(angle);
        return result;
    }

    // matriz de rotação ao longo do eixo Z
    static Matrix4 rotationZ(double angle) {
        Matrix4 result;
        result.m[0][0] = cos(angle);
        result.m[0][1] = -sin(angle);
        result.m[1][0] = sin(angle);
        result.m[1][1] = cos(angle);
        return result;
    }

    // Multiplica a matriz por um ponto, aplicando a transformação
    Point3 operator*(const Point3& p) const {
        return Point3(
            m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3],
            m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3],
            m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3]
        );
    }

    // Multiplica a matriz por um vetor, aplicando a transformação
    Vec3 operator*(const Vec3& v) const {
        return Vec3(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        );
    }

    // Multiplica duas matrizes 4x4
    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = 0;
                for (int k = 0; k < 4; ++k) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
};

#endif
