//
// Created by dbaumeister on 13.10.15.
//

#ifndef LBM_BLOCKSIMULATION_H
#define LBM_BLOCKSIMULATION_H

#include "D2Q9.h"

class BlockD2Q9 : public D2Q9{

public:
    BlockD2Q9(int dimX, int dimY)
            : D2Q9(dimX, dimY), fTmp((Real*) calloc(dimX * dimY * 9, sizeof(Real))){}

    ~BlockD2Q9() {
        free(fTmp);
    }

    void collide();
    void stream();
    Real* getArrayAfterCollision();

private:
    Real* fTmp;
};

#endif //LBM_BLOCKSIMULATION_H
