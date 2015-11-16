//
// Created by dbaumeister on 19.10.15.
//

#ifndef LBM_SWAPSIMULATION_H
#define LBM_SWAPSIMULATION_H

#include "D2Q9.h"

class SwapD2Q9 : public D2Q9{

public:
    SwapD2Q9(int dimX, int dimY)
            : D2Q9(dimX, dimY) {}


    ~SwapD2Q9() {}

    void collide();
    void stream();
};

#endif //LBM_SWAPSIMULATION_H
