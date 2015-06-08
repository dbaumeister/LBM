//
// Created by dbaumeister on 02.06.15.
//

#include <iostream>
#include "FluidSolver.h"

void FluidSolver::applyForces() {
    Vector3D gravity(0, 0.5f, 0);

    for(int i = 1; i < scene.getDimX() - 1; ++i){
        for(int j = 1; j < scene.getDimY() - 1; ++j){
                scene.vel(i,j) += (scene.getDt() * gravity);
        }
    }
}

void FluidSolver::advectVelocities(){
    VectorGrid advVel(scene.getDimX(), scene.getDimY());
    //TODO
    //scene.vel = advVel;
}

void FluidSolver::computeDivergence(RealGrid& div) {
    for (int i = 1; i < scene.getDimX() - 1; ++i) {
        for (int j = 1; j < scene.getDimY() - 1; ++j) {
            div.at(i, j) = scene.vel(i + 1, j).x - scene.vel(i, j).x
                                                   + scene.vel(i, j + 1).y - scene.vel(i, j).y;
        }
    }

}

void FluidSolver::computeOffDiag(RealGrid& offdiag){

    for (int i = 1; i < scene.getDimX() - 1; ++i) {
        for (int j = 1; j < scene.getDimY() - 1; ++j) {
            offdiag.at(i, j) = -1.f;
        }
    }
}

void FluidSolver::computeDiag(RealGrid& diag, RealGrid& offdiag){
    for (int i = 1; i < scene.getDimX() - 1; ++i) {
        for (int j = 1; j < scene.getDimY() - 1; ++j) {
            diag.at(i, j) = -1.f * (offdiag(i - 1, j) + offdiag(i + 1, j) + offdiag(i, j - 1) + offdiag(i, j + 1));
        }
    }
}


void FluidSolver::solvePressure(RealGrid& divergence, RealGrid& diag, RealGrid& offdiag){

}

void FluidSolver::correctVelocity(){
    for (int i = 1; i < scene.getDimX() - 1; ++i) {
        for (int j = 1; j < scene.getDimY() - 1; ++j) {
            scene.vel(i, j).x -= scene.pressure(i + 1, j) - scene.pressure(i, j);
            scene.vel(i, j).y -= scene.pressure(i, j + 1) - scene.pressure(i, j);
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

    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {
            scene.density(i, j) = advDen(i, j);
        }
    }

}

void FluidSolver::next() {
    applyForces();
    //advectVelocities();

    //RealGrid divergence(scene.getDimX(), scene.getDimY());
    //computeDivergence(divergence);

    //RealGrid offdiag(scene.getDimX(), scene.getDimY());
    //computeOffDiag(offdiag);

    //RealGrid diag(scene.getDimX(), scene.getDimY());
    //computeDiag(diag, offdiag);

    //solvePressure(divergence, diag, offdiag);

    //correctVelocity();
    advectDensity();
}
