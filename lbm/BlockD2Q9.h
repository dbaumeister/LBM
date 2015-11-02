//
// Created by dbaumeister on 13.10.15.
//

#ifndef LBM_SIMULATION_H
#define LBM_SIMULATION_H

#include "D2Q9.h"

class SimpleD2Q9 : public D2Q9{

public:
    SimpleD2Q9(int dimX, int dimY)
            : D2Q9(dimX, dimY), fTmp((Real*) calloc(dimX * dimY * 9, sizeof(Real))){}


    ~SimpleD2Q9() {
        free(fTmp);
    }

    void collide();
    void stream();
    Real* getArrayAfterCollision();

private:
    Real* fTmp;
};

#endif //LBM_SIMULATION_H