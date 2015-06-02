//
// Created by dbaumeister on 02.06.15.
//

#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include "Grid.h"


class Scene2D {
public:
    Scene2D(int xDim, int yDim)
            : xDim(xDim), yDim(yDim), vel(xDim, yDim), pressure(xDim, yDim), density(xDim, yDim) {}


    int getDimX(){
        return xDim;
    }

    int getDimY(){
        return yDim;
    }

private:
    int xDim, yDim;

    VectorGrid<Real> vel;
    RealGrid pressure;
    RealGrid density;
};

#endif //GRAPHICS_SCENE_H
