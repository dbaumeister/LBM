//
// Created by dbaumeister on 19.10.15.
//

#include "D2Q9.h"

/**
 * Allocates memory aligned to 64B,
 * which is one cache line on intel processors.
 */
double *calloc64ByteAligned(size_t size) {
    void* ptr = aligned_alloc(64 * sizeof(void*), size);
    if(ptr == nullptr) {
        std::cout << "ERROR: Memory allocation failed (" << size << "B)" << std::endl;
    }
    memset(ptr, 0, size);
    return (double*) ptr;
}

/**
 * Computes the density value rho.
 *
 * f points to the first entry of the distribution function,
 * no matter which data layout is used
 */
double computeRho(double* f) {
    double a = f[_SW] + f[_S] + f[_SE];
    double b = f[_W] + f[_C] + f[_E];
    double c = f[_NW] + f[_N] + f[_NE];
    return a + b + c;
}


/**
 * Computes the velocity entries u[0] and u[1] as well as u[0]*u[0] + u[1]*u[1].
 *
 * f points to the first entry of the distribution function.
 */
double computeUAndUSquare(double* f, double rho, double* u) {
    double vx = - f[_SW] + f[_SE] - f[_W] + f[_E]  - f[_NW] + f[_NE];
    double vy = - f[_SW] - f[_S] - f[_SE] + f[_NW] + f[_N] + f[_NE];

    u[0] = vx / rho;
    u[1] = vy / rho;
    return (vx * vx + vy * vy) / rho;
}


/**
 * Computes the local equilibrium for a given velocity.
 */

double computeLocalEquilibrium(const double weight, const int cx, const int cy, const double rho, const double* u, const double uSquare) {
    double c_u = cx * u[0] + cy * u[1];
    //cs = 1 / sqrt(3)
    return rho * weight * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
}

void D2Q9::init() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 3 * (iX * dimY + iY);
            f[i + _SW] = 1./36.;
            f[i + _S] = 1./9.;
            f[i + _SE] = 1./36.;
            f[i + _W] = 1./9.;
            f[i + _C] = 4./9.;
            f[i + _E] = 1./9.;
            f[i + _NW] = 1./36.;
            f[i + _N] = 1./9.;
            f[i + _NE] = 1./36.;
        }
    }
}

void D2Q9::getVel(VectorGrid& vel) {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {

            int i = 3 * (iX * dimY + iY);
            double u[2];
            double rho = computeRho(&f[i]);
            computeUAndUSquare(&f[i], rho, &u[0]);
            vel(iX, iY).x = 100 * u[0];
            vel(iX, iY).y = 100 * u[1];
        }
    }
}

D2Q9::D2Q9(int dimX, int dimY) : dimX(dimX), dimY(dimY),
                                 f(calloc64ByteAligned(dimX * dimY * 9 * sizeof(double))) {}

D2Q9::~D2Q9() {
    free(f);
}