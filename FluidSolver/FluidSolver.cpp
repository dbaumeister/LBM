//
// Created by dbaumeister on 02.06.15.
//

#include <iostream>
#include "FluidSolver.h"

void FluidSolver::applyForces() {
    Vector3D buoyancy(0, -0.5f, 0);
    buoyancy *= scene.getDt();

    for(int i = 1; i < scene.getDimX() - 1; ++i){
        for(int j = 1; j < scene.getDimY() - 1; ++j){
                scene.vel(i,j) += buoyancy * (scene.density(i, j) - 0.5f);
        }
    }
}

void FluidSolver::advectVelocities(){
    VectorGrid advVel(scene.getDimX(), scene.getDimY());

    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {

            //for the x-component:
            Real ib = i - scene.getDt() * scene.vel(i, j).x;
            Real jb = j - scene.getDt()
                          * (scene.vel(i - 1, j - 1).y + scene.vel(i - 1, j).y +  + scene.vel(i, j).y  + scene.vel(i, j - 1).y)
                          / 4.f; //Interpolation to get the right velocity at where the x-component is stored

            if(ib > 0 && ib < scene.getDimX() - 1 && jb > 0 && jb < scene.getDimY() - 1){

                Real ri = ib - floorf(ib);
                Real rj = jb - floorf(jb);

                Real d00 = (1.f - rj) * (1.f - ri) * scene.vel((int)ib, (int)jb).x;
                Real d10 = (1.f - rj) * ri * scene.vel((int)ib + 1, (int)jb).x;
                Real d01 = rj * (1.f - ri) * scene.vel((int)ib, (int)jb + 1).x;
                Real d11 = rj * ri * scene.vel((int)ib + 1, (int)jb + 1).x;

                advVel(i, j).x = d00 + d10 + d11 + d01;

            } else advVel(i, j).x = 0;


            //for the y-component:
            ib = i - scene.getDt()
                     * ( scene.vel(i, j).x + scene.vel(i + 1, j).x + scene.vel(i, j + 1).x + scene.vel(i + 1, j + 1).x)
                     / 4.f; //Interpolation to get the right velocity at where the y-component is stored
            jb = j - scene.getDt() * scene.vel(i, j).y;

            if(ib > 0 && ib < scene.getDimX() - 1 && jb > 0 && jb < scene.getDimY() - 1){

                Real ri = ib - floorf(ib);
                Real rj = jb - floorf(jb);

                Real d00 = (1.f - rj) * (1.f - ri) * scene.vel((int)ib, (int)jb).y;
                Real d10 = (1.f - rj) * ri * scene.vel((int)ib + 1, (int)jb).y;
                Real d01 = rj * (1.f - ri) * scene.vel((int)ib, (int)jb + 1).y;
                Real d11 = rj * ri * scene.vel((int)ib + 1, (int)jb + 1).y;

                advVel(i, j).y = d00 + d10 + d11 + d01;

            } else advVel(i, j).y = 0;
        }
    }
    scene.vel.set(advVel);
}

void FluidSolver::computeDivergence(RealGrid& div) {
    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {
            div.at(i, j) = scene.vel(i + 1, j).x - scene.vel(i, j).x
                                                   + scene.vel(i, j + 1).y - scene.vel(i, j).y;
        }
    }

}

void FluidSolver::computeOffDiag(RealGrid& offdiag){

    for (int j = 0; j < scene.getDimY(); ++j) {
        for (int i = 0; i < scene.getDimX(); ++i) {
            if(j > 0 && i > 0 &&  j < scene.getDimY() - 1 && i < scene.getDimX() - 1){
                offdiag.at(i, j) = -1.f;
            } else {
                offdiag.at(i, j) = 0.f;
            }
        }
    }
}

void FluidSolver::computeDiag(RealGrid& diag, RealGrid& offdiag){
    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {
            diag.at(i, j) = -offdiag(i - 1, j) - offdiag(i + 1, j) - offdiag(i, j - 1) - offdiag(i, j + 1);
        }
    }
}

Real FluidSolver::gridNorm(RealGrid& grid){
    Real sum = 0;
    for (int j = 0; j < scene.getDimY(); ++j) {
        for (int i = 0; i < scene.getDimX(); ++i) {
            sum += fabsf(grid(i, j));
        }
    }
    return sum;
}

void FluidSolver::applyIterativeStep(RealGrid& residual, RealGrid& divergence, RealGrid& diag, RealGrid& offdiag){
    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {
            Real r = - divergence(i, j)
                             - diag(i, j) * scene.pressure(i, j)
                             - offdiag(i - 1, j) * scene.pressure(i - 1, j)
                               - offdiag(i + 1, j) * scene.pressure(i + 1, j)
                                 - offdiag(i, j - 1) * scene.pressure(i, j - 1)
                                   - offdiag(i, j + 1) * scene.pressure(i, j + 1);

            residual(i, j) = r;
            scene.pressure(i, j) += 1.f / diag(i, j) * r;
        }
    }
}

void FluidSolver::solvePressure(RealGrid& divergence, RealGrid& diag, RealGrid& offdiag){
    //use scene.pressure
    RealGrid residual(scene.getDimX(), scene.getDimY());
    int n = 0;
    Real acc;
    for(; n < iterations; ++n){
        applyIterativeStep(residual, divergence, diag, offdiag);
        acc = gridNorm(residual);
        if(accuracy > acc) break;
    }
    std::cout << "Solve after " << n << " steps. (Accuracy: " << acc << ")" << std::endl;
}

void FluidSolver::correctVelocity(){
    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {
            scene.vel(i, j).x -= scene.pressure(i, j) - scene.pressure(i - 1, j);
            scene.vel(i, j).y -= scene.pressure(i, j) - scene.pressure(i, j - 1);
        }
    }
}

void FluidSolver::advectDensity(){
    RealGrid advDen(scene.getDimX(), scene.getDimY());

    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {

            Real ib = i - scene.getDt() * (scene.vel(i + 1, j).x + scene.vel(i, j).x) / 2.f;
            Real jb = j - scene.getDt() * (scene.vel(i, j + 1).y + scene.vel(i, j).y) / 2.f;

            if(ib > 0 && ib < scene.getDimX() - 1 && jb > 0 && jb < scene.getDimY() - 1){

                Real ri = ib - floorf(ib);
                Real rj = jb - floorf(jb);

                Real d00 = (1.f - rj) * (1.f - ri) * scene.density((int)ib, (int)jb);
                Real d10 = (1.f - rj) * ri * scene.density((int)ib + 1, (int)jb);
                Real d01 = rj * (1.f - ri) * scene.density((int)ib, (int)jb + 1);
                Real d11 = rj * ri * scene.density((int)ib + 1, (int)jb + 1);

                advDen.at(i, j) = d00 + d10 + d11 + d01;

            } else advDen.at(i, j) = 0;

        }
    }

    scene.density.set(advDen);

}


void FluidSolver::setWallVelocities() {
    int i;
    int j = 0;
    for (i = 0; i < scene.getDimX(); ++i) {
        scene.vel(i, j).x = 0;
        scene.vel(i, j).y = 0;
    }

    j = scene.getDimY() - 1;
    for (i = 0; i < scene.getDimX(); ++i) {
        scene.vel(i, j).x = 0;
        scene.vel(i, j).y = 0;
    }

    i = 0;
    for (j = 0; j < scene.getDimY(); ++j) {
        scene.vel(i, j).x = 0;
        scene.vel(i, j).y = 0;
    }

    i = scene.getDimX() - 1;
    for (j = 0; j < scene.getDimY(); ++j) {
        scene.vel(i, j).x = 0;
        scene.vel(i, j).y = 0;
    }

}


void FluidSolver::next() {
    advectVelocities();
    applyForces();
    setWallVelocities();

    RealGrid divergence(scene.getDimX(), scene.getDimY());
    computeDivergence(divergence);

    RealGrid offdiag(scene.getDimX(), scene.getDimY());
    computeOffDiag(offdiag);

    RealGrid diag(scene.getDimX(), scene.getDimY());
    computeDiag(diag, offdiag);

    solvePressure(divergence, diag, offdiag);

    correctVelocity();
    setWallVelocities();

    advectDensity();
}

