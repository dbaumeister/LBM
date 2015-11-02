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

            for (int iF = 0; iF < 9; ++iF) {

                int nextX = iX + c[iF][0];
                int nextY = iY + c[iF][1];
                int iNext = 9 * (nextX * dimY + nextY);

                if (nextX >= 0 && nextY >= 0 && nextX < dimX && nextY < dimY) {
                    f[i + iF] = fTmp[iNext + iF];
                }
            }
        }
    }
}


void SimpleD2Q9::seed() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 9 * (iX * dimY + iY);
            for(int iF = 0; iF < 9; ++iF) {
                f[i + iF] = 1.f / 9.f;
            }
            f[i + 6] = 0.2f;
        }
    }
}

void SimpleD2Q9::updateVelocitiesForGui() {
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

void SimpleD2Q9::next() {
    collide();
    stream();
    updateVelocitiesForGui();
}
