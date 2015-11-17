//
// Created by dbaumeister on 19.10.15.
//

#ifndef LBM_HELPERMETHODS_H
#define LBM_HELPERMETHODS_H

#include <stdlib.h>
#include <cstring>

#include "../general/Definitions.h"
#include "../scene/Grid.h"


//Datastructures
// CollOpt
// C SW S SE W NW N NE E
// Bundle
// SW S SE <-> W C E <-> NW N NE

static const int c[9][2] = {
        {0,0},
        {-1,1}, {-1,0}, {-1,-1}, {0,-1},
        {1,-1}, {1,0}, {1,1},
        {0,1}
};

static const double w[9] = { 4./9., 1./36., 1./9., 1./36., 1./9.,
                           1./36., 1./9., 1./36., 1./9. };

static const double omega = 0.7;

double *calloc64ByteAligned(size_t size);

double computeRho(double* f);

double computeUAndUSquare(double* f, double rho, double* u);

#if defined(BUNDLE) | defined(PROPOPT)
double computeLocalEquilibrium(const double weight, const int cx, const int cy, const double rho, const double* u, const double uSquare);
#endif
#ifdef COLLOPT
double computeLocalEquilibrium(int iF, double rho, double* u, double uSquare);
#endif

#ifdef BUNDLE
static const int _SW = 0;
static const int _S = 1;
static const int _SE = 2;
static const int _W = 3 * N;
static const int _C = 3 * N + 1;
static const int _E = 3 * N + 2;
static const int _NW = 6 * N;
static const int _N = 6 * N + 1;
static const int _NE = 6 * N + 2;
#endif

#ifdef PROPOPT
static const int _SW = 0;
static const int _S = N;
static const int _SE = 2 * N;
static const int _W = 3 * N;
static const int _C = 4 * N;
static const int _E = 5 * N;
static const int _NW = 6 * N;
static const int _N = 7 * N;
static const int _NE = 8 * N;
#endif

static const int half = 4; //(9-1)/2;

// Constant that defines the number of entries that are saved on one lattice.
// Typically this constant is 9 on a D2Q9 LBM.
static const int NUM_ENTRIES_PER_LATTICE = 9;

class D2Q9 {

public:
    D2Q9(int dimX, int dimY);

    virtual ~D2Q9();

    void init();

    virtual void collide() = 0;
    virtual void stream() = 0;

    void getVel(VectorGrid& vel);

protected:
    int dimX;
    int dimY;
    double* f;
};

#endif //LBM_HELPERMETHODS_H
