//
// Created by dbaumeister on 13.10.15.
//

#ifndef LBM_BLOCKSIMULATION_H
#define LBM_BLOCKSIMULATION_H

#include "D2Q9.h"

static const int BLOCK_SIZE = 8;

class BlockD2Q9 : public D2Q9{

public:
    BlockD2Q9(int dimX, int dimY)
            : D2Q9(dimX, dimY),
              fTmp(calloc64ByteAligned(dimX * dimY * NUM_ENTRIES_PER_LATTICE * sizeof(double))){}

    ~BlockD2Q9() {
        free(fTmp);
    }

    void collide();
    void stream();

private:
    double* fTmp;
};

#endif //LBM_BLOCKSIMULATION_H
