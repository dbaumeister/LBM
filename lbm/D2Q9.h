//
// Created by dbaumeister on 19.10.15.
//

#ifndef LBM_HELPERMETHODS_H
#define LBM_HELPERMETHODS_H

#include <stdlib.h>
#include <cstring>

#include "../general/Definitions.h"
#include "../scene/Grid.h"

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

double computeLocalEquilibrium(int iF, double rho, double* u, double uSquare);


bool eq(double a, double b);

static const int half = 4; //(9-1)/2;

// Constant that defines the number of entries that are saved on one lattice.
// Typically this constant is 9 on a D2Q9 LBM.
static const int NUM_ENTRIES_PER_LATTICE = 9;

class D2Q9 {

public:
    D2Q9(int dimX, int dimY) : dimX(dimX), dimY(dimY),
                               f(calloc64ByteAligned(dimX * dimY * NUM_ENTRIES_PER_LATTICE * sizeof(double))) {};

    ~D2Q9(){
        free(f);
    };

    void init();
    bool verify();

    virtual void collide() = 0;
    virtual void stream() = 0;
    virtual double* getArrayAfterCollision() = 0;

    void getVel(VectorGrid& vel);

protected:
    int dimX;
    int dimY;
    double* f;
};

#endif //LBM_HELPERMETHODS_H
