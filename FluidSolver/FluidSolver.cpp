//
// Created by dbaumeister on 02.06.15.
//

#include <iostream>
#include "FluidSolver.h"


void FluidSolver::applyForces() {
    Vector3D buoyancy(0, 0.9f, 0);
    buoyancy *= scene.getDt();

    for(int i = 1; i < scene.getDimX() - 1; ++i){
        for(int j = 1; j < scene.getDimY() - 1; ++j){
                scene.vel(i,j) += buoyancy * scene.density(i, j);
        }
    }
}

void FluidSolver::advectVelocities(){
    VectorGrid advVel(scene.getDimX(), scene.getDimY());

    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {

            //for the x-component:
            Real ib = i - scene.getDt() * scene.vel(i, j).x;
            Real jb = j - scene.getDt() * scene.vel(i, j).y;

            if(ib >= 0 && ib < scene.getDimX() - 1 && jb >= 0 && jb < scene.getDimY() - 1){

                advVel(i, j) = scene.vel.getInterpolated(ib, jb);

            } else advVel(i, j) = scene.vel.getDefaultValue();
      }
    }
    scene.vel.swap(advVel);
}

void FluidSolver::computeDivergence(RealGrid& divergence) {
    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {
            divergence(i, j) = 0.5f * (scene.vel(i + 1, j).x - scene.vel(i - 1, j).x
                                                   + scene.vel(i, j + 1).y - scene.vel(i, j - 1).y);
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
    Real acc = accuracy;
    for(; n < iterations; ++n){
        applyIterativeStep(residual, divergence, diag, offdiag);
        acc = gridNorm(residual);
        if(accuracy > acc) break;
    }
    //std::cout << "Solve after " << n << " steps. (Accuracy: " << acc << ")" << std::endl;
}

void FluidSolver::correctVelocity(){
    for (int j = 1; j < scene.getDimY() - 1; ++j) {
        for (int i = 1; i < scene.getDimX() - 1; ++i) {

            if(i == 1) {
                scene.vel(i, j).x -= scene.pressure(i + 1, j) - scene.pressure(i, j);
            }
            else if( i == scene.getDimX() - 2) {
                scene.vel(i, j).x = scene.pressure(i, j) - scene.pressure(i - 1, j);
            }
            else scene.vel(i, j).x -= 0.5f * (scene.pressure(i + 1, j) - scene.pressure(i - 1, j));


            if(j == 1) {
                scene.vel(i, j).y -= scene.pressure(i, j + 1) - scene.pressure(i, j);
            }
            else if(j == scene.getDimY() - 2) {
                scene.vel(i, j).y -= scene.pressure(i, j) - scene.pressure(i, j - 1);
            }
            else scene.vel(i, j).y -= 0.5f * (scene.pressure(i, j + 1) - scene.pressure(i, j - 1));
        }
    }
}


void FluidSolver::advectDensity(){
    RealGrid advDen(scene.getDimX(), scene.getDimY());
    int bnd = 1;
    for (int j = bnd; j < scene.getDimY() - bnd; ++j) {
        for (int i = bnd; i < scene.getDimX() - bnd; ++i) {

            Real ib = i - scene.getDt() * scene.vel(i, j).x;
            Real jb = j - scene.getDt() * scene.vel(i, j).y;

            if(ib >= 0 && ib < scene.getDimX() - 1 && jb >= 0 && jb < scene.getDimY() - 1){
                advDen(i, j) = scene.density.getInterpolated(ib, jb);
            } else advDen(i, j) = scene.density.getDefaultValue();
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

    advectDensity();
    advectVelocities();
    setWallVelocities();

    applyForces();

    RealGrid divergence(scene.getDimX(), scene.getDimY());
    computeDivergence(divergence);

    RealGrid offdiag(scene.getDimX(), scene.getDimY());
    computeOffDiag(offdiag);

    RealGrid diag(scene.getDimX(), scene.getDimY());
    computeDiag(diag, offdiag);

    solvePressure(divergence, diag, offdiag);

    correctVelocity();
    setWallVelocities();

}

