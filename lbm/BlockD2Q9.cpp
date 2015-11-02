//
// Created by dbaumeister on 13.10.15.
//

#include "BlockD2Q9.h"

void BlockD2Q9::collide() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {

            int i = NUM_ENTRIES_PER_LATTICE * (iX * dimY + iY);

            double u[2];
            double rho = computeRho(&f[i]);
            double uSquare = computeUAndUSquare(&f[i], rho, &u[0]);
            for (int iF = 0; iF < 9; ++iF) {
                fTmp[i + iF] = (1 - omega) * f[i + iF] + omega * computeLocalEquilibrium(iF, rho, &u[0], uSquare);
            }
        }
    }
}

void BlockD2Q9::stream() {
    for (int iX = 0; iX < (dimX + BLOCK_SIZE - 1) / BLOCK_SIZE; ++iX) {
        for (int iY = 0; iY < (dimY + BLOCK_SIZE - 1) / BLOCK_SIZE; ++iY) {

            //TODO
            for(int iXX = iX * BLOCK_SIZE; iXX < (iX + 1) * BLOCK_SIZE && iXX < dimX; ++iXX) {
                for (int iYY = iY * BLOCK_SIZE; iYY < (iY + 1) * BLOCK_SIZE && iYY < dimY; ++iYY) {

                    int i = NUM_ENTRIES_PER_LATTICE * (iXX * dimY + iYY);

                    f[i] = fTmp[i]; //iF = 0

                    for (int iF = 1; iF < 9; ++iF) {

                        int nextX = iXX + c[iF][0];
                        int nextY = iYY + c[iF][1];

                        int iNext = NUM_ENTRIES_PER_LATTICE * (nextX * dimY + nextY);

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
    }
}

double* BlockD2Q9::getArrayAfterCollision() {
    return fTmp;
}
