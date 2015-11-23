//
// Created by dbaumeister on 13.10.15.
//

#ifndef LBM_BLOCKSIMULATION_H
#define LBM_BLOCKSIMULATION_H

#include "D2Q9.h"

static const int BLOCK_SIZE = 8;

class OptD2Q9 : public D2Q9{

public:
    OptD2Q9(int dimX, int dimY)
            : D2Q9(dimX, dimY),
              fTmp(calloc64ByteAligned(dimX * dimY * 9 * sizeof(double))){}

    ~OptD2Q9() {
        free(fTmp);
    }

    void collide();
    void stream();

private:
    double* fTmp;
};

#endif //LBM_BLOCKSIMULATION_H
