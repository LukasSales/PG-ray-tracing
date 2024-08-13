#ifndef SOLVER_H
#define SOLVER_H

#include <cmath>
#include <complex>

int solveQuartic(const double coeffs[5], double roots[4]) {
    std::complex<double> coeff[4];
    std::complex<double> sol[4];

    // Converte coeficientes para números complexos
    for (int i = 0; i < 4; ++i) {
        coeff[i] = std::complex<double>(coeffs[i + 1] / coeffs[0], 0);
    }

    std::complex<double> i(0, 1); // Unidade imaginária

    // Inicializa soluções como zeros
    for (int i = 0; i < 4; ++i) {
        sol[i] = std::complex<double>(0, 0);
    }

    // Resolução da equação quártica
    std::complex<double> a2 = coeff[0];
    std::complex<double> a1 = coeff[1];
    std::complex<double> a0 = coeff[2];
    std::complex<double> b1 = coeff[3];

    std::complex<double> Q1 = (a2 * a2 - 3.0 * a1 + 12.0 * a0) / 9.0;
    std::complex<double> Q0 = (2.0 * a2 * a2 * a2 - 9.0 * a2 * a1 + 27.0 * a0) / 27.0;
    std::complex<double> delta = Q0 * Q0 - 4.0 * Q1 * Q1 * Q1;

    std::complex<double> Q = std::pow((Q0 + std::sqrt(delta)) / 2.0, 1.0 / 3.0);
    std::complex<double> S = std::sqrt(a2 * a2 / 4.0 - a1 + Q + Q1 / Q);

    sol[0] = -a2 / 4.0 + S / 2.0 + std::sqrt(-(3.0 * a2 * a2 / 4.0 - S * S - 2.0 * a1 + Q1 / Q)) / 2.0;
    sol[1] = -a2 / 4.0 + S / 2.0 - std::sqrt(-(3.0 * a2 * a2 / 4.0 - S * S - 2.0 * a1 + Q1 / Q)) / 2.0;
    sol[2] = -a2 / 4.0 - S / 2.0 + std::sqrt(-(3.0 * a2 * a2 / 4.0 - S * S - 2.0 * a1 - Q1 / Q)) / 2.0;
    sol[3] = -a2 / 4.0 - S / 2.0 - std::sqrt(-(3.0 * a2 * a2 / 4.0 - S * S - 2.0 * a1 - Q1 / Q)) / 2.0;

    int num_real_roots = 0;
    for (int i = 0; i < 4; ++i) {
        if (std::abs(sol[i].imag()) < 1e-6) {
            roots[num_real_roots++] = sol[i].real();
        }
    }

    return num_real_roots;
}

#endif // SOLVER_H
