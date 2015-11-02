//
// Created by dbaumeister on 19.10.15.
//

#include "D2Q9.h"

Real computeRho(Real* f) {
    Real rho = 0.f;
    for(int iF = 0; iF < 9; ++iF) {
        rho += f[iF];
    }
    return rho;
}

Real computeUAndUSquare(Real* f, Real rho, Real* u) {
    Real uSquare = 0.f;
    for(int iD = 0; iD < 2; ++iD) {
        u[iD] = 0.f;
        for(int iF = 0; iF < 9; ++iF) {
            u[iD] += f[iF] * c[iF][iD];
        }
        u[iD] /= rho;
        uSquare += u[iD] * u[iD];
    }
    return uSquare;
}

Real computeLocalEquilibrium(int iF, Real rho, Real* u, Real uSquare) {
    Real c_u = 0;
    for (int iD = 0; iD < 2; ++iD) {
        c_u += c[iF][iD] * u[iD];
    }
    //cs = 1 / sqrt(3)
    return rho * w[iF] * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
}

bool eq(float a, float b) {
    return (fabs(a - b) < 0.00001f);
}

VectorGrid &D2Q9::getVel() {
    return vel;
}


void D2Q9::seed() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 9 * (iX * dimY + iY);
            for(int iF = 0; iF < 9; ++iF) {
                f[i + iF] = w[iF];
            }
            f[i+ 6] -= 0.1f;
            f[i+ 2] += 0.1f;
        }
    }
}

void D2Q9::updateVelocitiesForGui() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 9 * (iX * dimY + iY);
            Real u[2];
            Real rho = computeRho(&f[i]);
            computeUAndUSquare(&f[i], rho, &u[0]);
            vel(iX, iY).x = 5 * u[0];
            vel(iX, iY).y = 5 * u[1];
        }
    }
}
