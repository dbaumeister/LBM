//
// Created by dbaumeister on 13.10.15.
//

#include "OptD2Q9.h"

void OptD2Q9::collision(double& rho, double* u, double& uSquare, int i) {
    rho = f[i + _SW] + f[i + _S] + f[i + _SE] + f[i + _W] + f[i + _C] + f[i + _E] + f[i + _NW] + f[i + _N] + f[i + _NE];
    u[0] = (- f[i + _SW] + f[i + _SE] - f[i + _W] + f[i + _E]  - f[i + _NW] + f[i + _NE]) / rho;
    u[1] = (- f[i + _SW] - f[i + _S] - f[i + _SE] + f[i + _NW] + f[i + _N] + f[i + _NE]) / rho;
    uSquare =  u[0] * u[0] + u[1] * u[1];

    f[i + _SW] = (1 - omega) * f[i + _SW] + omega * computeLocalEquilibrium(1. / 36., -1, -1, rho, &u[0], uSquare);
    f[i + _S] = (1 - omega) * f[i + _S] + omega * computeLocalEquilibrium(1. / 9., 0, -1, rho, &u[0], uSquare);
    f[i + _SE] = (1 - omega) * f[i + _SE] + omega * computeLocalEquilibrium(1. / 36., 1, -1, rho, &u[0], uSquare);
    f[i + _W] = (1 - omega) * f[i + _W] + omega * computeLocalEquilibrium(1. / 9., -1, 0, rho, &u[0], uSquare);
    f[i + _C] = (1 - omega) * f[i + _C] + omega * computeLocalEquilibrium(4. / 9., 0, 0, rho, &u[0], uSquare);
    f[i + _E] = (1 - omega) * f[i + _E] + omega * computeLocalEquilibrium(1. / 9., 1, 0, rho, &u[0], uSquare);
    f[i + _NW] = (1 - omega) * f[i + _NW] + omega * computeLocalEquilibrium(1. / 36., -1, 1, rho, &u[0], uSquare);
    f[i + _N] = (1 - omega) * f[i + _N] + omega * computeLocalEquilibrium(1. / 9., 0, 1, rho, &u[0], uSquare);
    f[i + _NE] = (1 - omega) * f[i + _NE] + omega * computeLocalEquilibrium(1. / 36., 1, 1, rho, &u[0], uSquare);
}

void OptD2Q9::collide() {
    double rho, u[2], uSquare;

    int i = 0;
    //lower left corner = SW
    collision(rho, &u[0], uSquare, i);
    fTmp[_NE] = f[_SW]; //BC
    fTmp[_N] = f[_S]; //BC
    fTmp[_NW] = f[_SE]; //BC
    fTmp[_E] = f[_W]; //BC
    fTmp[_C] = f[_C];
    fTmp[_E + 3 * dimY] = f[_E];
    fTmp[_SE] = f[_NW]; //BC
    fTmp[_N + 3] = f[_N];
    fTmp[_NE + 3 * dimY + 3] = f[_NE];

    //left boundary = W
    for(int iY = 1; iY < dimY - 1; ++iY) {
        i += 3;
        collision(rho, &u[0], uSquare, i);
        fTmp[i + _NE] = f[i + _SW]; //BC
        fTmp[i + _S - 3] = f[i + _S];
        fTmp[i + _SE + 3 * dimY - 3] = f[i + _SE];
        fTmp[i + _E] = f[i + _W]; //BC
        fTmp[i + _C] = f[i + _C];
        fTmp[i + _E + 3 * dimY] = f[i + _E];
        fTmp[i + _SE] = f[i + _NW]; //BC
        fTmp[i + _N + 3] = f[i + _N];
        fTmp[i + _NE + 3 * dimY + 3] = f[i + _NE];
    }

    //upper left corner = NW
    i += 3;
    collision(rho, &u[0], uSquare, i);
    fTmp[i + _NE] = f[i + _SW]; //BC
    fTmp[i + _S - 3] = f[i + _S];
    fTmp[i + _SE + 3 * dimY - 3] = f[i + _SE];
    fTmp[i + _E] = f[i + _W]; //BC
    fTmp[i + _C] = f[i + _C];
    fTmp[i + _E + 3 * dimY] = f[i + _E];
    fTmp[i + _S] = f[i + _N]; //BC
#ifdef CAVITY
    fTmp[i + _SE] = f[i + _NW] + 0.3/36.; //BC
    fTmp[i + _SW] = f[i + _NE] - 0.3/36.; //BC
#else
    fTmp[i + _SE] = f[i + _NW]; //BC
    fTmp[i + _SW] = f[i + _NE]; //BC
#endif

    //lower boundary = S
    i = 0;
    for(int iX = 1; iX < dimX - 1; ++iX) {
        i += 3 * dimY;
        collision(rho, &u[0], uSquare, i);
        fTmp[i + _NE] = f[i + _SW]; //BC
        fTmp[i + _N] = f[i + _S]; //BC
        fTmp[i + _NW] = f[i + _SE]; //BC
        fTmp[i + _W - 3 * dimY] = f[i + _W];
        fTmp[i + _C] = f[i + _C];
        fTmp[i + _E + 3 * dimY] = f[i + _E];
        fTmp[i + _NW - 3 * dimY + 3] = f[i + _NW];
        fTmp[i + _N + 3] = f[i + _N];
        fTmp[i + _NE + 3 * dimY + 3] = f[i + _NE];

    }


    //lower right corner = SE
    i += 3 * dimY;
    collision(rho, &u[0], uSquare, i);
    fTmp[i + _NE] = f[i + _SW]; //BC
    fTmp[i + _N] = f[i + _S]; //BC
    fTmp[i + _NW] = f[i + _SE]; //BC
    fTmp[i + _W - 3 * dimY] = f[i + _W];
    fTmp[i + _C] = f[i + _C];
    fTmp[i + _W] = f[i + _E]; //BC
    fTmp[i + _NW - 3 * dimY + 3] = f[i + _NW];
    fTmp[i + _N + 3] = f[i + _N];
    fTmp[i + _SW] = f[i + _NE]; //BC

    //inner block
    for (int iXX = 1; iXX < dimX - 1; iXX += BLOCK_SIZE) {
        for (int iYY = 1; iYY < dimY - 1; iYY += BLOCK_SIZE) {

            for(int iX = iXX; iX < std::min(iXX + BLOCK_SIZE, dimX - 1); ++iX) {
                for (int iY = iYY; iY < std::min(iYY + BLOCK_SIZE, dimY - 1); ++iY) {
                    int i = 3 * (iX * dimY + iY);
                    collision(rho, &u[0], uSquare, i);

                    //Stream update to neighbors
                    fTmp[i + _SW - 3 * dimY - 3] = f[i + _SW];
                    fTmp[i + _S - 3] = f[i + _S];
                    fTmp[i + _SE + 3 * dimY - 3] = f[i + _SE];
                    fTmp[i + _W - 3 * dimY] = f[i + _W];
                    fTmp[i + _C] = f[i + _C];
                    fTmp[i + _E + 3 * dimY] = f[i + _E];
                    fTmp[i + _NW - 3 * dimY + 3] = f[i + _NW];
                    fTmp[i + _N + 3] = f[i + _N];
                    fTmp[i + _NE + 3 * dimY + 3] = f[i + _NE];

                }
            }
        }
    }


    //right boundary = E
    for(int iY = 1; iY < dimY - 1; ++iY) {
        i += 3;
        collision(rho, &u[0], uSquare, i);
        fTmp[i + _SW - 3 * dimY - 3] = f[i + _SW];
        fTmp[i + _S - 3] = f[i + _S];
        fTmp[i + _NW] = f[i + _SE]; //BC
        fTmp[i + _W - 3 * dimY] = f[i + _W];
        fTmp[i + _C] = f[i + _C];
        fTmp[i + _W] = f[i + _E]; //BC
        fTmp[i + _NW - 3 * dimY + 3] = f[i + _NW];
        fTmp[i + _N + 3] = f[i + _N];
        fTmp[i + _SW] = f[i + _NE]; // BC

    }

    //upper right corner
    i += 3;
    collision(rho, &u[0], uSquare, i);
    fTmp[i + _SW - 3 * dimY - 3] = f[i + _SW];
    fTmp[i + _S - 3] = f[i + _S];
    fTmp[i + _NW] = f[i + _SE]; // BC
    fTmp[i + _W - 3 * dimY] = f[i + _W];
    fTmp[i + _C] = f[i + _C];
    fTmp[i + _W] = f[i + _E]; // BC

    fTmp[i + _S] = f[i + _N]; //BC

#ifdef CAVITY
    fTmp[i + _SE] = f[i + _NW] + 0.3/36. ; //BC
    fTmp[i + _SW] = f[i + _NE] - 0.3/36. ; //BC
#else
    fTmp[i + _SE] = f[i + _NW]; //BC
    fTmp[i + _SW] = f[i + _NE]; //BC
#endif

    //upper boundary = N
    i = 3 * (dimY - 1);
    for(int iX = 1; iX < dimX - 1; ++iX) {
        i += 3 * dimY;
        collision(rho, &u[0], uSquare, i);
        fTmp[i + _SW - 3 * dimY - 3] = f[i + _SW];
        fTmp[i + _S - 3] = f[i + _S];
        fTmp[i + _SE + 3 * dimY - 3] = f[i + _SE];
        fTmp[i + _W - 3 * dimY] = f[i + _W];
        fTmp[i + _C] = f[i + _C];
        fTmp[i + _E + 3 * dimY] = f[i + _E];
        fTmp[i + _S] = f[i + _N]; //BC
#ifdef CAVITY
        fTmp[i + _SE] = f[i + _NW] + 0.3/36.; //BC
        fTmp[i + _SW] = f[i + _NE] - 0.3/36.; //BC
#else
        fTmp[i + _SE] = f[i + _NW]; //BC
        fTmp[i + _SW] = f[i + _NE]; //BC
#endif
    }
}

void OptD2Q9::stream() {
    std::swap(f, fTmp);
}
