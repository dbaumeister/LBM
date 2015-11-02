//
// Created by dbaumeister on 19.10.15.
//

#include "SwapD2Q9.h"

void SwapD2Q9::collide() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {

            int i = NUM_ENTRIES_PER_LATTICE * (iX * dimY + iY);

            double u[2];
            double rho = computeRho(&f[i]);
            double uSquare = computeUAndUSquare(&f[i], rho, &u[0]);

            for (int iF = 0; iF < 9; ++iF) {
                f[i + iF] *= (1.-omega);
                f[i + iF] += omega * computeLocalEquilibrium(iF, rho, &u[0], uSquare);
            }
            for (int iF = 1; iF <= half; ++iF) {
                std::swap(f[i + iF], f[i + iF + half]);
            }
        }
    }
}

void SwapD2Q9::stream() {

    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {

            int i = NUM_ENTRIES_PER_LATTICE * (iX * dimY + iY);

            for (int iF = 1; iF <= half; ++iF) {
                int nextX = iX + c[iF][0];
                int nextY = iY + c[iF][1];

                int iNext = NUM_ENTRIES_PER_LATTICE * (nextX * dimY + nextY);

                if (nextX >= 0 && nextY >= 0 && nextY < dimY) {
                    //Note: small trick: removed "&& nextX < dimX" as nextX for the first half can only be lower than iX
                    std::swap(f[i + iF + half], f[iNext + iF]);
                } else { //Half-way bounce back BCs
                    std::swap(f[i + iF + half], f[i + iF]);
                }
            }
        }
    }
}

double* SwapD2Q9::getArrayAfterCollision() {
    return f;
}
