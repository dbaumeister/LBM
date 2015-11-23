//
// Created by dbaumeister on 13.10.15.
//

#include "OptD2Q9.h"

void OptD2Q9::collide() {
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

void OptD2Q9::stream() {
    std::swap(f, fTmp);
}