//
// Created by dbaumeister on 13.10.15.
//

#include "OptD2Q9.h"

static double wcorner = 1./36.;
static double wedge = 1./9.;
static double wcenter = 4./9.;

void OptD2Q9::collision(double& rho, double* u, double& uSquare, int i) {
    rho = f[i + _SW] + f[i + _S] + f[i + _SE] + f[i + _W] + f[i + _C] + f[i + _E] + f[i + _NW] + f[i + _N] + f[i + _NE];
    u[0] = (- f[i + _SW] + f[i + _SE] - f[i + _W] + f[i + _E]  - f[i + _NW] + f[i + _NE]) / rho;
    u[1] = (- f[i + _SW] - f[i + _S] - f[i + _SE] + f[i + _NW] + f[i + _N] + f[i + _NE]) / rho;
    uSquare =  u[0] * u[0] + u[1] * u[1];

    //cs = 1 / sqrt(3)
    double c_u = u[0] + u[1];
    double feq = rho * wcorner * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
    double feqinv = feq - 2 * rho * wcorner * 3.f * c_u;

    f[i + _NE] = omegaInv * f[i + _NE] + omega * feq;
    f[i + _SW] = omegaInv * f[i + _SW] + omega * feqinv;

    c_u = u[0] - u[1];
    feq = rho * wcorner * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
    feqinv = feq - 2 * rho * wcorner * 3.f * c_u;

    f[i + _SE] = omegaInv * f[i + _SE] + omega * feq;
    f[i + _NW] = omegaInv * f[i + _NW] + omega * feqinv;

    c_u = u[1];
    feq = rho * wedge * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
    feqinv = feq - 2 * rho * wedge * 3.f * c_u;

    f[i + _N] = omegaInv * f[i + _N] + omega * feq;
    f[i + _S] = omegaInv * f[i + _S] + omega * feqinv;


    c_u = u[0];
    feq = rho * wedge * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
    feqinv = feq - 2 * rho * wedge * 3.f * c_u;

    f[i + _E] = omegaInv * f[i + _E] + omega * feq;
    f[i + _W] = omegaInv * f[i + _W] + omega * feqinv;

    f[i + _C] = omegaInv * f[i + _C] + omega * rho * wcenter * (1.f - 1.5f * uSquare);

}

void OptD2Q9::next() {
    double rho, u[2], uSquare;
    int dimY3 = 3 * dimY;
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
        i += dimY3;
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
    i += dimY3;
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
        i += dimY3;
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

    //Swap array pointers
    std::swap(f, fTmp);
}
