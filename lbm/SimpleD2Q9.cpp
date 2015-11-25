//
// Created by dbaumeister on 13.10.15.
//

#include "SimpleD2Q9.h"

void SimpleD2Q9::collide() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 3 * (iX * dimY + iY);
            double u[2];
            double rho = computeRho(&f[i]);
            double uSquare = computeUAndUSquare(&f[i], rho, &u[0]);

            fTmp[i + _SW] = (1 - omega) * f[i + _SW] + omega * computeLocalEquilibrium(1./36., -1, -1, rho, &u[0], uSquare);
            fTmp[i + _S] = (1 - omega) * f[i + _S] + omega * computeLocalEquilibrium(1./9., 0, -1, rho, &u[0], uSquare);
            fTmp[i + _SE] = (1 - omega) * f[i + _SE] + omega * computeLocalEquilibrium(1./36., 1, -1, rho, &u[0], uSquare);
            fTmp[i + _W] = (1 - omega) * f[i + _W] + omega * computeLocalEquilibrium(1./9., -1, 0, rho, &u[0], uSquare);
            fTmp[i + _C] = (1 - omega) * f[i + _C] + omega * computeLocalEquilibrium(4./9., 0, 0, rho, &u[0], uSquare);
            fTmp[i + _E] = (1 - omega) * f[i + _E] + omega * computeLocalEquilibrium(1./9., 1, 0, rho, &u[0], uSquare);
            fTmp[i + _NW] = (1 - omega) * f[i + _NW] + omega * computeLocalEquilibrium(1./36., -1, 1, rho, &u[0], uSquare);
            fTmp[i + _N] = (1 - omega) * f[i + _N] + omega * computeLocalEquilibrium(1./9., 0, 1, rho, &u[0], uSquare);
            fTmp[i + _NE] = (1 - omega) * f[i  + _NE] + omega * computeLocalEquilibrium(1./36., 1, 1, rho, &u[0], uSquare);
        }
    }
}

void SimpleD2Q9::stream() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 3 * (iX * dimY + iY);

            //SW
            if(iX > 0 && iY > 0) {
                f[i + _SW] = fTmp[i + _SW - 3 * dimY - 3];
            }
            else {
                f[i + _SW] = fTmp[i + _NE]; // swap with NE
            }

            //S
            if(iY > 0) {
                f[i + _S] = fTmp[i + _S - 3];
            }
            else {
                f[i + _S] = fTmp[i + _N]; // swap with N
            }

            //SE
            if(iX < dimX - 1 && iY > 0) {
                f[i + _SE] = fTmp[i + _SE + 3 * dimY - 3];
            }
            else {
                f[i + _SE] = fTmp[i + _NW]; // swap with NW
            }


            //W
            if(iX > 0) {
                f[i + _W] = fTmp[i + _W - 3 * dimY];
            }
            else {
                f[i + _W] = fTmp[i + _E]; // swap with E
            }

            //C
            f[i + _C] = fTmp[i + _C];

            //E
            if(iX < dimX - 1) {
                f[i + _E] = fTmp[i + _E + 3 * dimY];
            }
            else {
                f[i + _E] = fTmp[i + _W]; // swap with W
            }

            //NW
            if(iX > 0 && iY < dimY - 1) {
                f[i + _NW] = fTmp[i + _NW - 3 * dimY + 3];
            }

#ifdef CAVITY
            else if(iY == dimY - 1){
                f[i + _NW] = fTmp[i + _SE] - 0.3/36.;
            }
#endif
            else {
                f[i + _NW] = fTmp[i + _SE]; // swap with SE
            }

            //N
            if(iY < dimY - 1) {
                f[i + _N] = fTmp[i + _N + 3];
            }
            else {
                f[i + _N] = fTmp[i + _S]; // swap with S
            }

            //NE
            if(iX < dimX - 1 && iY < dimY - 1) {
                f[i + _NE] = fTmp[i + _NE + 3 * dimY + 3];
            }
#ifdef CAVITY
            else if(iY == dimY - 1){
                f[i + _NE] = fTmp[i + _SW] + 0.3/36.;
            }
#endif
            else {
                f[i + _NE] = fTmp[i + _SW]; // swap with SW
            }
        }
    }
}

void SimpleD2Q9::next() {
    collide();
    stream();
}
