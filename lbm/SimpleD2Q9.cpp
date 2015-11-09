//
// Created by dbaumeister on 13.10.15.
//

#include "SimpleD2Q9.h"

void SimpleD2Q9::collide() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {

#ifdef BUNDLE
            int i = 3 * (iX * dimY + iY);
#else
            int i = NUM_ENTRIES_PER_LATTICE * (iX * dimY + iY);
#endif
            double u[2];
            double rho = computeRho(&f[i]);
            double uSquare = computeUAndUSquare(&f[i], rho, &u[0]);

#ifdef BUNDLE
            fTmp[i] = (1 - omega) * f[i] + omega * computeLocalEquilibrium(1./36., -1, -1, rho, &u[0], uSquare);
            fTmp[i + 1] = (1 - omega) * f[i + 1] + omega * computeLocalEquilibrium(1./9., 0, -1, rho, &u[0], uSquare);
            fTmp[i + 2] = (1 - omega) * f[i + 2] + omega * computeLocalEquilibrium(1./36., 1, -1, rho, &u[0], uSquare);
            fTmp[i + 3 * N] = (1 - omega) * f[i + 3 * N] + omega * computeLocalEquilibrium(1./9., -1, 0, rho, &u[0], uSquare);
            fTmp[i + 3 * N + 1] = (1 - omega) * f[i + 3 * N + 1] + omega * computeLocalEquilibrium(4./9., 0, 0, rho, &u[0], uSquare);
            fTmp[i + 3 * N + 2] = (1 - omega) * f[i + 3 * N + 2] + omega * computeLocalEquilibrium(1./9., 1, 0, rho, &u[0], uSquare);
            fTmp[i + 6 * N] = (1 - omega) * f[i + 6 * N] + omega * computeLocalEquilibrium(1./36., -1, 1, rho, &u[0], uSquare);
            fTmp[i + 6 * N + 1] = (1 - omega) * f[i + 6 * N + 1] + omega * computeLocalEquilibrium(1./9., 0, 1, rho, &u[0], uSquare);
            fTmp[i + 6 * N + 2] = (1 - omega) * f[i  + 6 * N + 2] + omega * computeLocalEquilibrium(1./36., 1, 1, rho, &u[0], uSquare);
#else
            for (int iF = 0; iF < 9; ++iF) {
                fTmp[i + iF] = (1 - omega) * f[i + iF] + omega * computeLocalEquilibrium(iF, rho, &u[0], uSquare);
            }
#endif
        }
    }
}

void SimpleD2Q9::stream() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {

#ifdef BUNDLE
            int i = 3 * (iX * dimY + iY);

            //SW
            if(iX > 0 && iY > 0) {
                f[i] = fTmp[i - 3 * dimY - 3];
            }
            else {
                f[i] = fTmp[i + 6 * N + 2]; // swap with NE
            }

            //S
            if(iX > 0) {
                f[i + 1] = fTmp[i + 1 - 3 * dimY];
            }
            else {
                f[i + 1] = fTmp[i + 1 + 6 * N]; // swap with N
            }

            //SE
            if(iX < dimX - 1 && iY > 0) {
                f[i + 2] = fTmp[i + 2 + 3 * dimY - 3];
            }
            else {
                f[i + 2] = fTmp[i + 6 * N]; // swap with NW
            }


            //W
            if(iX > 0) {
                f[i + 3 * N] = fTmp[i + 3 * N - 3 * dimY];
            }
            else {
                f[i + 3 * N] = fTmp[i + 3 * N + 2]; // swap with E
            }

            //C
            f[i + 3 * N + 1] = fTmp[i + 3 * N + 1];

            //E
            if(iX < dimX - 1) {
                f[i + 3 * N + 2] = fTmp[i + 3 * N + 2 + 3 * dimY];
            }
            else {
                f[i + 3 * N + 2] = fTmp[i + 3 * N]; // swap with W
            }

            //NW
            if(iX > 0 && iY < dimY - 1) {
                f[i + 6 * N] = fTmp[i + 6 * N - 3 * dimY + 3];
            }

#ifdef CAVITY
            else if(iY == dimY - 1){
                f[i + 6 * N] = fTmp[i + 2] - 0.3/36.;
            }
#endif
            else {
                f[i + 6 * N] = fTmp[i + 2]; // swap with SE
            }

            //N
            if(iY < dimY - 1) {
                f[i + 6 * N + 1] = fTmp[i + 6 * N + 1 + 3];
            }
            else {
                f[i + 6 * N + 1] = f[i + 1]; // swap with S
            }

            //NE
            if(iX < dimX - 1 && iY < dimY - 1) {
                f[i + 6 * N + 2] = fTmp[i + 6 * N + 2 + 3 * dimY + 3];
            }
#ifdef CAVITY
            else if(iY == dimY - 1){
                f[i + 6 * N + 2] = fTmp[i] + 0.3/36.;
            }
#endif
            else {
                f[i + 6 * N + 2] = fTmp[i]; // swap with SW
            }



#else
            int i = NUM_ENTRIES_PER_LATTICE * (iX * dimY + iY);

            f[i] = fTmp[i]; //iF = 0

            for (int iF = 1; iF < 9; ++iF) {

                int nextX = iX + c[iF][0];
                int nextY = iY + c[iF][1];
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
#endif
        }
    }
}

double* SimpleD2Q9::getArrayAfterCollision() {
    return fTmp;
}
