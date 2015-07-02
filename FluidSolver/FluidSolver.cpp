//
// Created by dbaumeister on 02.06.15.
//

#include <iostream>
#include "FluidSolver.h"

void FluidSolver::applyForces() {
    Vector3D buoyancy(0, 0.5f, 0);
    buoyancy *= scene.getDt();

    for(int i = 1; i < scene.getDimX() - 1; ++i){
        for(int j = 1; j < scene.getDimY() - 1; ++j){
                //scene.vel(i,j) += buoyancy * (scene.density(i, j) - 0.5f);
        }
    }
}

//TODO koordinatensystem betrachen
void FluidSolver::advectVelocities(){
    VectorGrid advVel(scene.getDimX(), scene.getDimY());

    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {

            //for the x-component:
            Real ib = (i - 0.5f) - scene.getDt() * scene.vel(i, j).x;
            Real jb = j - scene.getDt()
                          * (scene.vel(i - 1, j - 1).y + scene.vel(i - 1, j).y +  + scene.vel(i, j).y  + scene.vel(i, j - 1).y)
                          / 4.f; //Interpolation to get the right velocity at where the x-component is stored

            int in = advVel.getIndex(ib);
            int jn = advVel.getIndex(jb);

            if(in >= 0 && in < scene.getDimX() - 1 && jn >= 0 && jn < scene.getDimY() - 1){

                Real di = ib - in;
                Real dj = jb - jn;

                Real d00 = (1.f - dj) * (1.f - di) * scene.vel(in, jn).x;
                Real d10 = (1.f - dj) * di * scene.vel(in + 1, jn).x;
                Real d01 = dj * (1.f - di) * scene.vel(in, jn + 1).x;
                Real d11 = dj * di * scene.vel(in + 1, jn + 1).x;

                advVel(i, j).x = d00 + d10 + d11 + d01;

            } else advVel(i, j).x = 0;


            //for the y-component:
            ib = i - scene.getDt()
                     * ( scene.vel(i, j).x + scene.vel(i + 1, j).x + scene.vel(i, j + 1).x + scene.vel(i + 1, j + 1).x)
                     / 4.f; //Interpolation to get the right velocity at where the y-component is stored
            jb = (j - 0.5f) - scene.getDt() * scene.vel(i, j).y;

            in = advVel.getIndex(ib);
            jn = advVel.getIndex(jb);

            if(in >= 0 && in < scene.getDimX() - 1 && jn >= 0 && jn < scene.getDimY() - 1){

                Real di = ib - in;
                Real dj = jb - jn;

                Real d00 = (1.f - dj) * (1.f - di) * scene.vel(in, jn).y;
                Real d10 = (1.f - dj) * di * scene.vel(in + 1, jn).y;
                Real d01 = dj * (1.f - di) * scene.vel(in, jn + 1).y;
                Real d11 = dj * di * scene.vel(in + 1, jn + 1).y;

                advVel(i, j).y = d00 + d10 + d11 + d01;

            } else advVel(i, j).y = 0;
      }
    }
    scene.vel.swap(advVel);
}

void FluidSolver::computeDivergence(RealGrid& divergence) {
    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {
            divergence(i, j) = scene.vel(i + 1, j).x - scene.vel(i, j).x
                                                   + scene.vel(i, j + 1).y - scene.vel(i, j).y;
        }
    }

}

void FluidSolver::computeOffDiag(RealGrid& offdiag){

    for (int j = 0; j < scene.getDimY(); ++j) {
        for (int i = 0; i < scene.getDimX(); ++i) {
            if(j > 0 && i > 0 &&  j < scene.getDimY() - 1 && i < scene.getDimX() - 1){
                offdiag(i, j) = -1.f;
            } else {
                offdiag(i, j) = 0.f;
            }
        }
    }
}

void FluidSolver::computeDiag(RealGrid& diag, RealGrid& offdiag){
    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {
            diag(i, j) = -offdiag(i - 1, j) - offdiag(i + 1, j) - offdiag(i, j - 1) - offdiag(i, j + 1);
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

            Real ib = (Real)i - scene.getDt() * (scene.vel(i + 1, j).x + scene.vel(i, j).x) / 2.f;
            Real jb = (Real)j - scene.getDt() * (scene.vel(i, j + 1).y + scene.vel(i, j).y) / 2.f;

            int in = advDen.getIndex(ib);
            int jn = advDen.getIndex(jb);

            if(in >= 0 && in < scene.getDimX() - 1 && jn >= 0 && jn < scene.getDimY() - 1){

                Real di = ib - in;
                Real dj = jb - jn;

                Real d00 = (1.f - dj) * (1.f - di) * scene.density(in, jn);
                Real d10 = (1.f - dj) * di * scene.density(in + 1, jn);
                Real d01 = dj * (1.f - di) * scene.density(in, jn + 1);
                Real d11 = dj * di * scene.density(in + 1, jn + 1);

                advDen(i, j) = d00 + d10 + d11 + d01;

            } else advDen(i, j) = 0;

        }
    }

    scene.density.swap(advDen);

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

