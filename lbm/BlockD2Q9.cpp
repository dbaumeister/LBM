//
// Created by dbaumeister on 13.10.15.
//

#include "BlockD2Q9.h"

#ifdef BUNDLE
void BlockD2Q9::collide() {
    for (int iXX = 0; iXX < dimX; iXX += BLOCK_SIZE) {
        for (int iYY = 0; iYY < dimY; iYY += BLOCK_SIZE) {

            for(int iX = iXX; iX < std::min(iXX + BLOCK_SIZE, dimX); ++iX) {
                for (int iY = iYY; iY < std::min(iYY + BLOCK_SIZE, dimY); ++iY) {
                    int i = 3 * (iX * dimY + iY);
                    double u[2];
                    double rho = computeRho(&f[i]);
                    double uSquare = computeUAndUSquare(&f[i], rho, &u[0]);

                    f[i + _SW] = (1 - omega) * f[i + _SW] +
                                    omega * computeLocalEquilibrium(1. / 36., -1, -1, rho, &u[0], uSquare);
                    f[i + _S] = (1 - omega) * f[i + _S] +
                                   omega * computeLocalEquilibrium(1. / 9., 0, -1, rho, &u[0], uSquare);
                    f[i + _SE] = (1 - omega) * f[i + _SE] +
                                    omega * computeLocalEquilibrium(1. / 36., 1, -1, rho, &u[0], uSquare);
                    f[i + _W] = (1 - omega) * f[i + _W] +
                                   omega * computeLocalEquilibrium(1. / 9., -1, 0, rho, &u[0], uSquare);
                    f[i + _C] = (1 - omega) * f[i + _C] +
                                   omega * computeLocalEquilibrium(4. / 9., 0, 0, rho, &u[0], uSquare);
                    f[i + _E] = (1 - omega) * f[i + _E] +
                                   omega * computeLocalEquilibrium(1. / 9., 1, 0, rho, &u[0], uSquare);
                    f[i + _NW] = (1 - omega) * f[i + _NW] +
                                    omega * computeLocalEquilibrium(1. / 36., -1, 1, rho, &u[0], uSquare);
                    f[i + _N] = (1 - omega) * f[i + _N] +
                                   omega * computeLocalEquilibrium(1. / 9., 0, 1, rho, &u[0], uSquare);
                    f[i + _NE] = (1 - omega) * f[i + _NE] +
                                    omega * computeLocalEquilibrium(1. / 36., 1, 1, rho, &u[0], uSquare);


                    //Stream update to neighbors

                    //SW
                    if(iX > 0 && iY > 0) {
                        fTmp[i + _SW - 3 * dimY - 3] = f[i + _SW];
                    }
                    else {
                        fTmp[i + _NE] = f[i + _SW];
                    }

                    //S
                    if(iY > 0) {
                        fTmp[i + _S - 3] = f[i + _S];
                    }
                    else {
                        fTmp[i + _N] = f[i + _S];
                    }

                    //SE
                    if(iX < dimX - 1 && iY > 0) {
                        fTmp[i + _SE + 3 * dimY - 3] = f[i + _SE];
                    }
                    else {
                        fTmp[i + _NW] = f[i + _SE];
                    }


                    //W
                    if(iX > 0) {
                        fTmp[i + _W - 3 * dimY] = f[i + _W];
                    }
                    else {
                        fTmp[i + _E] = f[i + _W];
                    }

                    //C
                    fTmp[i + _C] = f[i + _C];

                    //E
                    if(iX < dimX - 1) {
                        fTmp[i + _E + 3 * dimY] = f[i + _E];
                    }
                    else {
                        fTmp[i + _W] = f[i + _E];
                    }

                    //NW
                    if(iX > 0 && iY < dimY - 1) {
                        fTmp[i + _NW - 3 * dimY + 3] = f[i + _NW];
                    }

#ifdef CAVITY
                    else if(iY == dimY - 1) {
                        fTmp[i + _SE] = f[i + _NW] + 0.3/36.;
                    }
#endif
                    else {
                        fTmp[i + _SE] = f[i + _NW];
                    }

                    //N
                    if(iY < dimY - 1) {
                        fTmp[i + _N + 3] = f[i + _N];
                    }
                    else {
                        fTmp[i + _S] = f[i + _N]; // swap with S
                    }

                    //NE
                    if(iX < dimX - 1 && iY < dimY - 1) {
                        fTmp[i + _NE + 3 * dimY + 3] = f[i + _NE];
                    }
#ifdef CAVITY
                    else if(iY == dimY - 1) {
                        fTmp[i + _SW] = f[i + _NE] - 0.3/36.;
                    }
#endif
                    else {
                        fTmp[i + _SW] = f[i + _NE]; // swap with SW
                    }
                }
            }
        }
    }
}

void BlockD2Q9::stream() {
    std::swap(f, fTmp);
}
#endif



#ifdef COLLOPT
void BlockD2Q9::collide() {
    for (int iXX = 0; iXX < dimX; iXX += BLOCK_SIZE) {
        for (int iYY = 0; iYY < dimY; iYY += BLOCK_SIZE) {

            for(int iX = iXX; iX < std::min(iXX + BLOCK_SIZE, dimX); ++iX) {
                for (int iY = iYY; iY < std::min(iYY + BLOCK_SIZE, dimY); ++iY) {
                    int i = NUM_ENTRIES_PER_LATTICE * (iX * dimY + iY);
                    double u[2];
                    double rho = computeRho(&f[i]);
                    double uSquare = computeUAndUSquare(&f[i], rho, &u[0]);

                    for (int iF = 0; iF < 9; ++iF) {
                        f[i + iF] = (1 - omega) * f[i + iF] + omega * computeLocalEquilibrium(iF, rho, &u[0], uSquare);
                    }

                    //Stream update to neighbors
                    fTmp[i] = f[i]; //iF = 0

                    for (int iF = 1; iF < 9; ++iF) {

                        int nextX = iX + c[iF][0];
                        int nextY = iY + c[iF][1];
                        int iNext = NUM_ENTRIES_PER_LATTICE * (nextX * dimY + nextY);

                        int opposite = half * (-1 + 2 * (iF <= half)); // - half if (iF > half)

                        if (nextX >= 0 && nextX < dimX && nextY >= 0 && nextY < dimY) {
                            fTmp[iNext + iF] = f[i + iF];
                        }
#ifdef CAVITY
                        else if (nextY >= dimY) { //Cavity
                            fTmp[i + iF + opposite] = f[i + iF] - 0.3f * w[iF] * c[iF][0];
                            // 0.3f = 2.f * 0.05f * 3.f
                            // 2.f == from formular
                            // 0.05f == added velocity in x-direction each iteration
                            // 3.f == 1 / cs²
                        }
#endif
                        else { //Half-way bounce back BCs
                            fTmp[i + iF + opposite] = f[i + iF];
                        }
                    }
                }
            }
        }
    }
}

void BlockD2Q9::stream() {
    std::swap(f, fTmp);
}
#endif

#ifdef PROPOPT
void BlockD2Q9::collide() {
    for (int iXX = 0; iXX < dimX; iXX += BLOCK_SIZE) {
        for (int iYY = 0; iYY < dimY; iYY += BLOCK_SIZE) {

            for(int iX = iXX; iX < std::min(iXX + BLOCK_SIZE, dimX); ++iX) {
                for (int iY = iYY; iY < std::min(iYY + BLOCK_SIZE, dimY); ++iY) {
                    int i = iX * dimY + iY;
                    double u[2];
                    double rho = computeRho(&f[i]);
                    double uSquare = computeUAndUSquare(&f[i], rho, &u[0]);

                    f[i + _SW] = (1 - omega) * f[i + _SW] +
                                 omega * computeLocalEquilibrium(1. / 36., -1, -1, rho, &u[0], uSquare);
                    f[i + _S] = (1 - omega) * f[i + _S] +
                                omega * computeLocalEquilibrium(1. / 9., 0, -1, rho, &u[0], uSquare);
                    f[i + _SE] = (1 - omega) * f[i + _SE] +
                                 omega * computeLocalEquilibrium(1. / 36., 1, -1, rho, &u[0], uSquare);
                    f[i + _W] = (1 - omega) * f[i + _W] +
                                omega * computeLocalEquilibrium(1. / 9., -1, 0, rho, &u[0], uSquare);
                    f[i + _C] = (1 - omega) * f[i + _C] +
                                omega * computeLocalEquilibrium(4. / 9., 0, 0, rho, &u[0], uSquare);
                    f[i + _E] = (1 - omega) * f[i + _E] +
                                omega * computeLocalEquilibrium(1. / 9., 1, 0, rho, &u[0], uSquare);
                    f[i + _NW] = (1 - omega) * f[i + _NW] +
                                 omega * computeLocalEquilibrium(1. / 36., -1, 1, rho, &u[0], uSquare);
                    f[i + _N] = (1 - omega) * f[i + _N] +
                                omega * computeLocalEquilibrium(1. / 9., 0, 1, rho, &u[0], uSquare);
                    f[i + _NE] = (1 - omega) * f[i + _NE] +
                                 omega * computeLocalEquilibrium(1. / 36., 1, 1, rho, &u[0], uSquare);


                    //Stream update to neighbors

                    //SW
                    if(iX > 0 && iY > 0) {
                        fTmp[i + _SW - dimY - 1] = f[i + _SW];
                    }
                    else {
                        fTmp[i + _NE] = f[i + _SW];
                    }

                    //S
                    if(iY > 0) {
                        fTmp[i + _S - 1] = f[i + _S];
                    }
                    else {
                        fTmp[i + _N] = f[i + _S];
                    }

                    //SE
                    if(iX < dimX - 1 && iY > 0) {
                        fTmp[i + _SE + dimY - 1] = f[i + _SE];
                    }
                    else {
                        fTmp[i + _NW] = f[i + _SE];
                    }


                    //W
                    if(iX > 0) {
                        fTmp[i + _W - dimY] = f[i + _W];
                    }
                    else {
                        fTmp[i + _E] = f[i + _W];
                    }

                    //C
                    fTmp[i + _C] = f[i + _C];

                    //E
                    if(iX < dimX - 1) {
                        fTmp[i + _E + dimY] = f[i + _E];
                    }
                    else {
                        fTmp[i + _W] = f[i + _E];
                    }

                    //NW
                    if(iX > 0 && iY < dimY - 1) {
                        fTmp[i + _NW - dimY + 1] = f[i + _NW];
                    }
#ifdef CAVITY
                    else if(iY == dimY - 1) {
                        fTmp[i + _SE] = f[i + _NW] + 0.3/36.;
                    }
#endif
                    else {
                        fTmp[i + _SE] = f[i + _NW];
                    }

                    //N
                    if(iY < dimY - 1) {
                        fTmp[i + _N + 1] = f[i + _N];
                    }
                    else {
                        fTmp[i + _S] = f[i + _N]; // swap with S
                    }

                    //NE
                    if(iX < dimX - 1 && iY < dimY - 1) {
                        fTmp[i + _NE + dimY + 1] = f[i + _NE];
                    }
#ifdef CAVITY
                    else if(iY == dimY - 1) {
                        fTmp[i + _SW] = f[i + _NE] - 0.3/36.;
                    }
#endif
                    else {
                        fTmp[i + _SW] = f[i + _NE]; // swap with SW
                    }
                }
            }
        }
    }
}

void BlockD2Q9::stream() {
    std::swap(f, fTmp);
}
#endif