//
// Created by dbaumeister on 02.06.15.
//

#ifndef GRAPHICS_FLUIDSOLVER_H
#define GRAPHICS_FLUIDSOLVER_H

#include "../Scene/Scene.h"

class FluidSolver {
public:
    FluidSolver(Scene2D& scene, Real dt = 0.1f) : scene(scene) {}

    void next();

private:
    Scene2D& scene;


    void applyForces();
    void advectVelocities();
    void computeDivergence(RealGrid& div);
    void computeOffDiag(RealGrid& offdiag);
    void computeDiag(RealGrid& diag, RealGrid& offdiag);
    void solvePressure(RealGrid& divergence, RealGrid& diag, RealGrid& offdiag);
    void correctVelocity();
    void advectDensity();
};

#endif //GRAPHICS_FLUIDSOLVER_H
