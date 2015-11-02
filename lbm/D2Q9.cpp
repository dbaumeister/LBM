//
// Created by dbaumeister on 19.10.15.
//

#include "D2Q9.h"

Real computeRho(Real* f) {
    Real rho = 0.f;
    for(int iF = 0; iF < 9; ++iF) {
        rho += f[iF];
    }
    return rho;
}

Real computeUAndUSquare(Real* f, Real rho, Real* u) {
    Real uSquare = 0.f;
    for(int iD = 0; iD < 2; ++iD) {
        u[iD] = 0.f;
        for(int iF = 0; iF < 9; ++iF) {
            u[iD] += f[iF] * c[iF][iD];
        }
        u[iD] /= rho;
        uSquare += u[iD] * u[iD];
    }
    return uSquare;
}

Real computeLocalEquilibrium(int iF, Real rho, Real* u, Real uSquare) {
    Real c_u = 0;
    for (int iD = 0; iD < 2; ++iD) {
        c_u += c[iF][iD] * u[iD];
    }
    //cs = 1 / sqrt(3)
    return rho * w[iF] * (1.f + 3.f * c_u + 4.5f * c_u * c_u - 1.5f * uSquare);
}


void D2Q9::init() {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 9 * (iX * dimY + iY);
            for(int iF = 0; iF < 9; ++iF) {
                f[i + iF] = w[iF];
            }
        }
    }
}

void D2Q9::getVel(VectorGrid& vel) {
    for (int iX = 0; iX < dimX; ++iX) {
        for (int iY = 0; iY < dimY; ++iY) {
            int i = 9 * (iX * dimY + iY);
            Real u[2];
            Real rho = computeRho(&f[i]);
            computeUAndUSquare(&f[i], rho, &u[0]);
            vel(iX, iY).x = 100 * u[0];
            vel(iX, iY).y = 100 * u[1];
        }
    }
}

bool eq(float a, float b) {
    return (fabs(a - b) < 0.00001f);
}

bool D2Q9::verify() {
    // Verify that collision does not change the impulse

    Real* fPrior = f;
    Real* fAfter = getArrayAfterCollision();

    for(int iX = 0; iX < dimX; ++iX) {
        for(int iY = 0; iY < dimY; ++iY) {
            int i = 9 * (iX * dimY + iY);

            Real uPrior[2];
            Real rhoPrior = computeRho(&fPrior[i]);
            Real uSquarePrior = computeUAndUSquare(&fPrior[i], rhoPrior, &uPrior[0]);

            // p = || m * v || = || rho * V * v || = //Note: Volume V == 1 * 1 * 1 == 1
            // || rho * v || = (rho * v[0])² + (rho * v[1])² = rho² * (v[0]² + v[1]²) = rho² * vSquare
            Real impulsePrior = rhoPrior * rhoPrior * uSquarePrior;

            collide();
            Real uAfter[2];
            Real rhoAfter = computeRho(&fAfter[i]);
            Real uSquareAfter = computeUAndUSquare(&fAfter[i], rhoAfter, &uAfter[0]);

            Real impulseAfter = rhoAfter * rhoAfter * uSquareAfter;

            if(!eq(impulsePrior, impulseAfter)) {
                printf("Wrong collision in Lattice [%d, %d]: Prior (%f) - (%f, %f) <-> After (%f) - (%f, %f).\n",
                       iX, iY, rhoPrior, uPrior[0], uPrior[1], rhoAfter, uAfter[0], uAfter[1]);
                return false;
            }

        }
    }

    // Verify that the streaming step streams the values into the correct cells
    for(int iX = 0; iX < dimX; ++iX) {
        for(int iY = 0; iY < dimY; ++iY) {
            int i = 9 * (iX * dimY + iY);

            for(int iF = 0; iF < 9; ++iF) {
                fAfter[i + iF] = iF;
            }
        }
    }
    stream();
    for(int iX = 1; iX < dimX - 1; ++iX) {
        for(int iY = 1; iY < dimY - 1; ++iY) {
            int i = 9 * (iX * dimY + iY);

            for(int iF = 0; iF < 9; ++iF) {
                if(!eq(fPrior[i + iF], iF)) {
                    printf("Wrong streaming in Lattice [%d, %d]: f[%d] = %f]\n",
                           iX, iY, iF, f[i + iF]);
                    return false;
                }
            }

        }
    }
    return true;
}
