//
// Created by dbaumeister on 19.10.15.
//

#ifndef LBM_HELPERMETHODS_H
#define LBM_HELPERMETHODS_H

#include <stdlib.h>
#include <cstring>

#include "../general/Definitions.h"
#include "../scene/Grid.h"

static const double omega = 0.7;
static const double omegaInv = 1 - omega;

double *calloc64ByteAligned(size_t size);

double computeRho(double* f);

double computeUAndUSquare(double* f, double rho, double* u);

double computeLocalEquilibrium(const double weight, const int cx, const int cy, const double rho, const double* u, const double uSquare);

static const int _SW = 0;
static const int _S = 1;
static const int _SE = 2;
static const int _W = 3 * N;
static const int _C = 3 * N + 1;
static const int _E = 3 * N + 2;
static const int _NW = 6 * N;
static const int _N = 6 * N + 1;
static const int _NE = 6 * N + 2;

class D2Q9 {

public:
    D2Q9(int dimX, int dimY);

    virtual ~D2Q9();

    void init();
    virtual void next() = 0;

    void getVel(VectorGrid& vel);

protected:
    const int dimX;
    const int dimY;
    double* f;
};

#endif //LBM_HELPERMETHODS_H
