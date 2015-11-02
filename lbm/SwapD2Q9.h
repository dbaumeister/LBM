//
// Created by dbaumeister on 19.10.15.
//

#ifndef LBM_SWAPSIMULATION_H
#define LBM_SWAPSIMULATION_H

#include "Helper.h"
#include "../scene/Grid.h"

class SwapD2Q9 : public D2Q9{

public:
    SwapD2Q9(int dimX, int dimY)
            : D2Q9(dimX, dimY), f((Real*) calloc(dimX * dimY * 9, sizeof(Real))){}


    ~SwapD2Q9() {
        free(f);
    }

    void seed();
    void next();

private:
    Real* f;

    void collide();
    void stream();
    void updateVelocitiesForGui();
};

#endif //LBM_SWAPSIMULATION_H
