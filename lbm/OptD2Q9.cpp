//
// Created by dbaumeister on 13.10.15.
//

#include "OptD2Q9.h"

static const double wcorner = 1./36.;
static const double wedge = 1./9.;
static const double wcenter = 4./9.;

static const double omegaInv = 1 - omega;

void OptD2Q9::collision(double& rho, double* u, double& uSquare, const int i) {
    rho = f[i + _SW] + f[i + _S] + f[i + _SE] + f[i + _W] + f[i + _C] + f[i + _E] + f[i + _NW] + f[i + _N] + f[i + _NE];
    u[0] = (- f[i + _SW] + f[i + _SE] - f[i + _W] + f[i + _E]  - f[i + _NW] + f[i + _NE]) / rho;
    u[1] = (- f[i + _SW] - f[i + _S] - f[i + _SE] + f[i + _NW] + f[i + _N] + f[i + _NE]) / rho;
    uSquare =  u[0] * u[0] + u[1] * u[1];

    //cs = 1 / sqrt(3)
    double c_u = u[0] + u[1];
    double feq = rho * wcorner * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
    double feqinv = feq - 2 * rho * wcorner * 3.f * c_u;

    double t1 = omegaInv * f[i + _NE] + omega * feq;
    f[i + _NE]  = omegaInv * f[i + _SW] + omega * feqinv;
    f[i + _SW] = t1;

    c_u = u[0] - u[1];
    feq = rho * wcorner * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
    feqinv = feq - 2 * rho * wcorner * 3.f * c_u;

    double t2 = omegaInv * f[i + _SE] + omega * feq;
    f[i + _SE] = omegaInv * f[i + _NW] + omega * feqinv;
    f[i + _NW] = t2;

    c_u = u[1];
    feq = rho * wedge * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
    feqinv = feq - 2 * rho * wedge * 3.f * c_u;

    double t3 = omegaInv * f[i + _N] + omega * feq;
    f[i + _N] = omegaInv * f[i + _S] + omega * feqinv;
    f[i + _S] = t3;


    c_u = u[0];
    feq = rho * wedge * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
    feqinv = feq - 2 * rho * wedge * 3.f * c_u;

    double t4 = omegaInv * f[i + _E] + omega * feq;
    f[i + _E] = omegaInv * f[i + _W] + omega * feqinv;
    f[i + _W] = t4;

    f[i + _C] = omegaInv * f[i + _C] + omega * rho * wcenter * (1.f - 1.5f * uSquare);
}

void OptD2Q9::next() {
    double rho, u[2], uSquare;
    int dimY3 = 3 * dimY;
    int i = 0;

    //lower left corner = SW
    collision(rho, &u[0], uSquare, i);

    //left boundary = W
    for(int iY = 1; iY < dimY - 1; ++iY) {
        i += 3;
        collision(rho, &u[0], uSquare, i);
        std::swap(f[i + _N], f[i + _S - 3]);
    }

    //upper left corner = NW
    i += 3;
    collision(rho, &u[0], uSquare, i);
    std::swap(f[i + _N], f[i + _S - 3]);
#ifdef CAVITY
    f[i + _SE] += 0.3/36.; //BC
    f[i + _SW] -= 0.3/36.; //BC
#endif

    //lower boundary = S
    i = 0;
    for(int iX = 1; iX < dimX - 1; ++iX) {
        i += dimY3;
        collision(rho, &u[0], uSquare, i);
        std::swap(f[i + _E], f[i + _W - dimY3]);
    }

    //inner block
    for(int iX = 1; iX < dimX - 1; ++iX) {
        int i = iX * dimY3;
        for (int iY = 1; iY < dimY - 1; ++iY) {

            // X * *
            // X o *
            // X X *
            // Only swap with X neighbors, because they have already been accessed

            i += 3;
            collision(rho, &u[0], uSquare, i);
            std::swap(f[i + _E], f[i + _W - dimY3]);
            std::swap(f[i + _NE], f[i + _SW - dimY3 - 3]);
            std::swap(f[i + _SE], f[i + _NW - dimY3 + 3]);
            std::swap(f[i + _N], f[i + _S - 3]);
        }

        //upper boundary = N
        i += 3;
        collision(rho, &u[0], uSquare, i);
        std::swap(f[i + _E], f[i + _W - dimY3]);
        std::swap(f[i + _NE], f[i + _SW - dimY3 - 3]);
        std::swap(f[i + _N], f[i + _S - 3]);
#ifdef CAVITY
        f[i + _SE] += 0.3/36.; //BC
        f[i + _SW] -= 0.3/36.; //BC
#endif

    }

    //lower right corner = SE
    i += dimY3;
    collision(rho, &u[0], uSquare, i);
    std::swap(f[i + _E], f[i + _W - dimY3]);
    std::swap(f[i + _SE], f[i + _NW - dimY3 + 3]);

    //right boundary = E
    for(int iY = 1; iY < dimY - 1; ++iY) {
        i += 3;
        collision(rho, &u[0], uSquare, i);
        std::swap(f[i + _E], f[i + _W - dimY3]);
        std::swap(f[i + _NE], f[i + _SW - dimY3 - 3]);
        std::swap(f[i + _SE], f[i + _NW - dimY3 + 3]);
        std::swap(f[i + _N], f[i + _S - 3]);

    }

    //upper right corner
    i += 3;
    collision(rho, &u[0], uSquare, i);
    std::swap(f[i + _E], f[i + _W - dimY3]);
    std::swap(f[i + _NE], f[i + _SW - dimY3 - 3]);
    std::swap(f[i + _N], f[i + _S - 3]);

#ifdef CAVITY
    f[i + _SE] += 0.3/36.; //BC
    f[i + _SW] -= 0.3/36.; //BC
#endif
}
