//
// Created by dbaumeister on 19.10.15.
//

#include "SwapD2Q9.h"

#ifdef BUNDLE
void SwapD2Q9::collide() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 3 * (iX * dimY + iY);
            double u[2];
            double rho = computeRho(&f[i]);
            double uSquare = computeUAndUSquare(&f[i], rho, &u[0]);

            f[i + _SW] = (1 - omega) * f[i + _SW] + omega * computeLocalEquilibrium(1./36., -1, -1, rho, &u[0], uSquare);
            f[i + _S] = (1 - omega) * f[i + _S] + omega * computeLocalEquilibrium(1./9., 0, -1, rho, &u[0], uSquare);
            f[i + _SE] = (1 - omega) * f[i + _SE] + omega * computeLocalEquilibrium(1./36., 1, -1, rho, &u[0], uSquare);
            f[i + _W] = (1 - omega) * f[i + _W] + omega * computeLocalEquilibrium(1./9., -1, 0, rho, &u[0], uSquare);
            f[i + _C] = (1 - omega) * f[i + _C] + omega * computeLocalEquilibrium(4./9., 0, 0, rho, &u[0], uSquare);
            f[i + _E] = (1 - omega) * f[i + _E] + omega * computeLocalEquilibrium(1./9., 1, 0, rho, &u[0], uSquare);
            f[i + _NW] = (1 - omega) * f[i + _NW] + omega * computeLocalEquilibrium(1./36., -1, 1, rho, &u[0], uSquare);
            f[i + _N] = (1 - omega) * f[i + _N] + omega * computeLocalEquilibrium(1./9., 0, 1, rho, &u[0], uSquare);
            f[i + _NE] = (1 - omega) * f[i  + _NE] + omega * computeLocalEquilibrium(1./36., 1, 1, rho, &u[0], uSquare);

            std::swap(f[i + _SW], f[i + _NE]); //SW & NE
            std::swap(f[i + _S], f[i + _N]); //S & N
            std::swap(f[i + _SE], f[i + _NW]); //SE & NW
            std::swap(f[i + _W], f[i + _E]); //W & E
        }
    }
}

void SwapD2Q9::stream() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 3 * (iX * dimY + iY);

            if(iX > 0 && iY > 0) {
                std::swap(f[i + _SW], f[i + _NE - 3 * dimY - 3]); // swap SW with NE of the [iX - 1, iY - 1] field
            }


            if(iY > 0) {
                std::swap(f[i + _S], f[i + _N - 3]); // swap S with N of the [iX, iY - 1] field
            }


            if(iX < dimX - 1 && iY > 0) {
                std::swap(f[i + _SE], f[i + _NW + 3 * dimY - 3]); // swap SE with NW of the [iX + 1, iY - 1] field
            }

            if(iX > 0) {
                std::swap(f[i + _W], f[i + _E - 3 * dimY]); // swap W with E of the [iX - 1, iY] field
            }
        }
    }
}
#endif

#ifdef COLLOPT
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
        }
    }
}
#endif

#ifdef PROPOPT
void SwapD2Q9::collide() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = iX * dimY + iY;
            double u[2];
            double rho = computeRho(&f[i]);
            double uSquare = computeUAndUSquare(&f[i], rho, &u[0]);

            f[i + _SW] = (1 - omega) * f[i + _SW] + omega * computeLocalEquilibrium(1./36., -1, -1, rho, &u[0], uSquare);
            f[i + _S] = (1 - omega) * f[i + _S] + omega * computeLocalEquilibrium(1./9., 0, -1, rho, &u[0], uSquare);
            f[i + _SE] = (1 - omega) * f[i + _SE] + omega * computeLocalEquilibrium(1./36., 1, -1, rho, &u[0], uSquare);
            f[i + _W] = (1 - omega) * f[i + _W] + omega * computeLocalEquilibrium(1./9., -1, 0, rho, &u[0], uSquare);
            f[i + _C] = (1 - omega) * f[i + _C] + omega * computeLocalEquilibrium(4./9., 0, 0, rho, &u[0], uSquare);
            f[i + _E] = (1 - omega) * f[i + _E] + omega * computeLocalEquilibrium(1./9., 1, 0, rho, &u[0], uSquare);
            f[i + _NW] = (1 - omega) * f[i + _NW] + omega * computeLocalEquilibrium(1./36., -1, 1, rho, &u[0], uSquare);
            f[i + _N] = (1 - omega) * f[i + _N] + omega * computeLocalEquilibrium(1./9., 0, 1, rho, &u[0], uSquare);
            f[i + _NE] = (1 - omega) * f[i  + _NE] + omega * computeLocalEquilibrium(1./36., 1, 1, rho, &u[0], uSquare);

            std::swap(f[i + _SW], f[i + _NE]); //SW & NE
            std::swap(f[i + _S], f[i + _N]); //S & N
            std::swap(f[i + _SE], f[i + _NW]); //SE & NW
            std::swap(f[i + _W], f[i + _E]); //W & E
        }
    }
}

void SwapD2Q9::stream() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = iX * dimY + iY;

            if(iX > 0 && iY > 0) {
                std::swap(f[i + _SW], f[i + _NE - dimY - 1]); // swap SW with NE of the [iX - 1, iY - 1] field
            }


            if(iY > 0) {
                std::swap(f[i + _S], f[i + _N - 1]); // swap S with N of the [iX, iY - 1] field
            }


            if(iX < dimX - 1 && iY > 0) {
                std::swap(f[i + _SE], f[i + _NW + dimY - 1]); // swap SE with NW of the [iX + 1, iY - 1] field
            }

            if(iX > 0) {
                std::swap(f[i + _W], f[i + _E - dimY]); // swap W with E of the [iX - 1, iY] field
            }
        }
    }
}
#endif