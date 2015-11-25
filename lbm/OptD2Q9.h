//
// Created by dbaumeister on 13.10.15.
//

#ifndef LBM_BLOCKSIMULATION_H
#define LBM_BLOCKSIMULATION_H

#include "D2Q9.h"

class OptD2Q9 : public D2Q9{

public:
    OptD2Q9(int dimX, int dimY)
            : D2Q9(dimX, dimY){}

    ~OptD2Q9() {}

    void next();

private:
    //Collision is always done the same -> inlined function to avoid return jumps
    inline void collision(double& rho, double* u, double& uSquare, int i);
};

#endif //LBM_BLOCKSIMULATION_H
