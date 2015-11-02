//
// Created by dbaumeister on 19.10.15.
//

#ifndef LBM_HELPERMETHODS_H
#define LBM_HELPERMETHODS_H

#include "../general/Definitions.h"
#include "../scene/Grid.h"

static const int c[9][2] = {
        {0,0},
        {-1,1}, {-1,0}, {-1,-1}, {0,-1},
        {1,-1}, {1,0}, {1,1},
        {0,1}
};

static const Real w[9] = { 4.f/9.f, 1.f/36.f, 1.f/9.f, 1.f/36.f, 1.f/9.f,
                           1.f/36.f, 1.f/9.f, 1.f/36.f, 1.f/9.f };

static const Real omega = 0.7f;

Real computeRho(Real* f);

Real computeUAndUSquare(Real* f, Real rho, Real* u);

Real computeLocalEquilibrium(int iF, Real rho, Real* u, Real uSquare);

bool eq(float a, float b);

static const int half = 4; //(9-1)/2;

class D2Q9 {

public:
    D2Q9(int dimX, int dimY) : dimX(dimX), dimY(dimY), f((Real*) calloc(dimX * dimY * 9, sizeof(Real))) {};

    ~D2Q9(){
        free(f);
    };

    void init();
    bool verify();

    virtual void collide() = 0;
    virtual void stream() = 0;
    virtual Real* getArrayAfterCollision() = 0;

    void getVel(VectorGrid& vel);

protected:
    int dimX;
    int dimY;
    Real* f;
};

#endif //LBM_HELPERMETHODS_H
