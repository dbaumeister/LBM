//
// Created by dbaumeister on 02.06.15.
//

#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include "../General/Definitions.h"
#include "../VectorMath/Vector.h"
#include "Grid.h"


class Scene2D {
public:
    Scene2D(int xDim, int yDim, Real dt = 1.0)
            : xDim(xDim), yDim(yDim), vel(xDim, yDim), dt(dt), pressure(xDim, yDim), density(xDim, yDim) {}


    inline int getDimX(){
        return xDim;
    }

    inline int getDimY(){
        return yDim;
    }

    inline Real getDt() {
        return dt;
    }

    VectorGrid vel;

    RealGrid pressure;
    RealGrid density;

private:
    Real dt;
    int xDim, yDim;
};


void applyDensityBlock(Scene2D& scene, int x, int y, int width, int height, Real val);

void applyVelocityBlock(Scene2D& scene, int x, int y, int width, int height, Vector3D val);

#endif //GRAPHICS_SCENE_H
