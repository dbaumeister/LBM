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
#ifdef BUNDLE
    double a = f[0] + f[1] + f[2];
    double b = f[3 * N] + f[3 * N + 1] + f[3 * N + 2];
    double c = f[6 * N] + f[6 * N + 1] + f[6 * N + 2];
    return a + b + c;
#else
    double rho = 0.;
    for(int iF = 0; iF < 9; ++iF) {
        rho += f[iF];
    }
    return rho;
#endif
}


/**
 * Computes the velocity entries u[0] and u[1] as well as u[0]*u[0] + u[1]*u[1].
 *
 * f points to the first entry of the distribution function,
 * no matter which data layout is used
 */
double computeUAndUSquare(double* f, double rho, double* u) {
#ifdef BUNDLE
    double vx = - f[0] + f[2] - f[3 * N] + f[3 * N + 2]  - f[6 * N] + f[6 * N + 2];
    double vy = - f[0] - f[1] - f[2] + f[6 * N ] + f[6 * N + 1] + f[6 * N + 2];

    u[0] = vx / rho;
    u[1] = vy / rho;
    return (vx * vx + vy * vy) / rho;
#else
    double uSquare = 0.f;
    for(int iD = 0; iD < 2; ++iD) {
        u[iD] = 0.f;

        for(int iF = 0; iF < 9; ++iF) {
            u[iD] += f[iF] * c[iF][iD];
        }
        u[iD] /= rho;
        uSquare += u[iD] * u[iD];
    }
    return uSquare;
#endif
}



#ifdef BUNDLE
/**
 * Computes the local equilibrium for a given velocity.
 */
double computeLocalEquilibrium(double weight, int cx, int cy, double rho, double* u, double uSquare) {
    double c_u = cx * u[0] + cy * u[1];
    //cs = 1 / sqrt(3)
    return rho * weight * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
}
#else
/**
 * Computes the local equilibrium for a given velocity.
 */
double computeLocalEquilibrium(int iF, double rho, double* u, double uSquare) {
    double c_u = 0;
    for (int iD = 0; iD < 2; ++iD) {
        c_u += c[iF][iD] * u[iD];
    }
    //cs = 1 / sqrt(3)
    return rho * w[iF] * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
}
#endif



void D2Q9::init() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
#ifdef BUNDLE
            int i = 3 * (iX * dimY + iY);
            f[i] = 1./36.;
            f[i + 1] = 1./9.;
            f[i + 2] = 1./36.;
            f[i + 3 * N] = 1./9.;
            f[i + 3 * N + 1] = 4./9.;
            f[i + 3 * N + 2] = 1./9.;
            f[i + 6 * N] = 1./36.;
            f[i + 6 * N + 1] = 1./9.;
            f[i + 6 * N + 2] = 1./36.;
#else
            int i = NUM_ENTRIES_PER_LATTICE * (iX * dimY + iY);

            for(int iF = 0; iF < 9; ++iF) {
                f[i + iF] = w[iF];
            }
#endif
        }
    }
}

void D2Q9::getVel(VectorGrid& vel) {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {

#ifdef BUNDLE
            int i = 3 * (iX * dimY + iY);
#else
            int i = NUM_ENTRIES_PER_LATTICE * (iX * dimY + iY);
#endif
            double u[2];
            double rho = computeRho(&f[i]);
            computeUAndUSquare(&f[i], rho, &u[0]);
            vel(iX, iY).x = 100 * u[0];
            vel(iX, iY).y = 100 * u[1];
        }
    }
}

bool eq(double a, double b) {
    return (fabs(a - b) < 0.0000001);
}

bool D2Q9::verify() {
    // Verify that collision does not change the impulse
#ifdef BUNDLE
    //TODO
#else
    double* fPrior = f;
    double* fAfter = getArrayAfterCollision();

    for(int iX = 0; iX < dimX; ++iX) {
        for(int iY = 0; iY < dimY; ++iY) {

            int i = NUM_ENTRIES_PER_LATTICE * (iX * dimY + iY);

            double uPrior[2];
            double rhoPrior = computeRho(&fPrior[i]);
            double uSquarePrior = computeUAndUSquare(&fPrior[i], rhoPrior, &uPrior[0]);

            // p = || m * v || = || rho * V * v || = //Note: Volume V == 1 * 1 * 1 == 1
            // || rho * v || = (rho * v[0])² + (rho * v[1])² = rho² * (v[0]² + v[1]²) = rho² * vSquare
            double impulsePrior = rhoPrior * rhoPrior * uSquarePrior;

            collide();
            double uAfter[2];
            double rhoAfter = computeRho(&fAfter[i]);
            double uSquareAfter = computeUAndUSquare(&fAfter[i], rhoAfter, &uAfter[0]);

            double impulseAfter = rhoAfter * rhoAfter * uSquareAfter;

            if(!eq(impulsePrior, impulseAfter)) {
                printf("Wrong collision in Lattice [%d, %d]: Prior (%f) - (%f, %f) <-> After (%f) - (%f, %f).\n",
                       iX, iY, rhoPrior, uPrior[0], uPrior[1], rhoAfter, uAfter[0], uAfter[1]);
                return false;
            }

        }
    }
#endif
    return true;
}
