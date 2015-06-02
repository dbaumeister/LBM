//
// Created by dbaumeister on 02.06.15.
//

#ifndef GRAPHICS_FLUIDSOLVER_H
#define GRAPHICS_FLUIDSOLVER_H

#include "../Scene/Scene.h"

class FluidSolver {
public:
    FluidSolver(Scene2D& scene) : scene(scene) {}

    void next();

private:
    Scene2D& scene;
};

#endif //GRAPHICS_FLUIDSOLVER_H
