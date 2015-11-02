//
// Created by dbaumeister on 13.10.15.
//

#include "SimpleD2Q9.h"

void SimpleD2Q9::collide() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 9 * (iX * dimY + iY);
            Real u[2];
            Real rho = computeRho(&f[i]);
            Real uSquare = computeUAndUSquare(&f[i], rho, &u[0]);
            for (int iF = 0; iF < 9; ++iF) {
                fTmp[i + iF] = (1 - omega) * f[i + iF] + omega * computeLocalEquilibrium(iF, rho, &u[0], uSquare);
            }
        }
    }
}

void SimpleD2Q9::stream() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 9 * (iX * dimY + iY);

            f[i] = fTmp[i]; //iF = 0

            for (int iF = 1; iF < 9; ++iF) {
                int nextX = iX + c[iF][0];
                int nextY = iY + c[iF][1];
                int iNext = 9 * (nextX * dimY + nextY);

                int opposite = half * (-1 + 2 * (iF <= half)); // - half if (iF > half)

                if (nextX >= 0 && nextX < dimX && nextY >= 0 && nextY < dimY) {
                    f[i + iF] = fTmp[iNext + iF];
                }
#ifdef CAVITY
                else if (nextY >= dimY) { //Cavity
                    f[i + iF] = fTmp[i + iF + opposite] + 0.3f * w[iF] * c[iF][0];
                    // 0.3f = 2.f * 0.05f * 3.f
                    // 2.f == from formular
                    // 0.05f == added velocity in x-direction each iteration
                    // 3.f == 1 / cs²
                }
#endif
                else { //Half-way bounce back BCs
                    f[i + iF] = fTmp[i + iF + opposite];
                }
            }
        }
    }
}

Real* SimpleD2Q9::getArrayAfterCollision() {
    return fTmp;
}
