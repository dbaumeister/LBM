//
// Created by dbaumeister on 13.10.15.
//

#ifndef LBM_SIMULATION_H
#define LBM_SIMULATION_H

#include "Helper.h"

class SimpleD2Q9 : public D2Q9{

public:
    SimpleD2Q9(int dimX, int dimY)
            : D2Q9(dimX, dimY), f((Real*) calloc(2 * dimX * dimY * 9, sizeof(Real))),
              fTmp(f + dimX * dimY * 9){}


    ~SimpleD2Q9() {
        free(f);
    }

    void seed();
    void next();

private:
    Real* f;
    Real* fTmp;

    void collide();
    void stream();
    void updateVelocitiesForGui();
};

#endif //LBM_SIMULATION_H
