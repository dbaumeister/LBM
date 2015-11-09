//
// Created by dbaumeister on 19.10.15.
//

#include "SwapD2Q9.h"

void SwapD2Q9::collide() {
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
            f[i] = (1 - omega) * f[i] + omega * computeLocalEquilibrium(1./36., -1, -1, rho, &u[0], uSquare);
            f[i + 1] = (1 - omega) * f[i + 1] + omega * computeLocalEquilibrium(1./9., 0, -1, rho, &u[0], uSquare);
            f[i + 2] = (1 - omega) * f[i + 2] + omega * computeLocalEquilibrium(1./36., 1, -1, rho, &u[0], uSquare);
            f[i + 3 * N] = (1 - omega) * f[i + 3 * N] + omega * computeLocalEquilibrium(1./9., -1, 0, rho, &u[0], uSquare);
            f[i + 3 * N + 1] = (1 - omega) * f[i + 3 * N + 1] + omega * computeLocalEquilibrium(4./9., 0, 0, rho, &u[0], uSquare);
            f[i + 3 * N + 2] = (1 - omega) * f[i + 3 * N + 2] + omega * computeLocalEquilibrium(1./9., 1, 0, rho, &u[0], uSquare);
            f[i + 6 * N] = (1 - omega) * f[i + 6 * N] + omega * computeLocalEquilibrium(1./36., -1, 1, rho, &u[0], uSquare);
            f[i + 6 * N + 1] = (1 - omega) * f[i + 6 * N + 1] + omega * computeLocalEquilibrium(1./9., 0, 1, rho, &u[0], uSquare);
            f[i + 6 * N + 2] = (1 - omega) * f[i  + 6 * N + 2] + omega * computeLocalEquilibrium(1./36., 1, 1, rho, &u[0], uSquare);

            std::swap(f[i], f[i + 6 * N + 2]); //SW & NE
            std::swap(f[i + 1], f[i + 6 * N + 1]); //S & N
            std::swap(f[i + 2], f[i + 6 * N]); //SE & NW
            std::swap(f[i + 3 * N], f[i + 3 * N + 2]); //W & E
#else
            for (int iF = 0; iF < 9; ++iF) {
                f[i + iF] *= (1.-omega);
                f[i + iF] += omega * computeLocalEquilibrium(iF, rho, &u[0], uSquare);
            }
            for (int iF = 1; iF <= half; ++iF) {
                std::swap(f[i + iF], f[i + iF + half]);
            }
#endif
        }
    }
}

void SwapD2Q9::stream() {

    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {

#ifdef BUNDLE
            int i = 3 * (iX * dimY + iY);

            if(iX > 0 && iY > 0) {
                std::swap(f[i], f[i + 6 * N - 3 * dimY - 3]); // swap SW with NE of the [iX - 1, iY - 1] field
            }


            if(iY > 0) {
                std::swap(f[i + 1], f[i + 1 + 6 * N - 3]); // swap S with N of the [iX, iY - 1] field
            }


            if(iX < dimX - 1 && iY > 0) {
                std::swap(f[i + 2], f[i + 6 * N + 3 * dimY - 3]); // swap SE with NW of the [iX + 1, iY - 1] field
            }

            if(iX > 0) {
                std::swap(f[i + 3 * N], f[i + 3 * N - 3 * dimY]); // swap W with E of the [iX - 1, iY] field
            }

#else
            int i = NUM_ENTRIES_PER_LATTICE * (iX * dimY + iY);
            //{-1,1}, {-1,0}, {-1,-1}, {0,-1}
            for (int iF = 1; iF <= half; ++iF) {
                int nextX = iX + c[iF][0];
                int nextY = iY + c[iF][1];

                int iNext = NUM_ENTRIES_PER_LATTICE * (nextX * dimY + nextY);

                if (nextX >= 0 && nextY >= 0 && nextY < dimY) {
                    //Note: small trick: removed "&& nextX < dimX" as nextX for the first half can only be lower than iX
                    std::swap(f[i + iF + half], f[iNext + iF]);
                }
            }
#endif
        }
    }
}

double* SwapD2Q9::getArrayAfterCollision() {
    return f;
}
