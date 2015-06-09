#include <iostream>

#include "GUI/GUI.h"
#include "FluidSolver/FluidSolver.h"


#define TIMESTEP 1.0/60


int main(void)
{

    Scene2D scene(64, 64);

    //source
    int sheight = scene.getDimY() * 0.05;
    int swidth = scene.getDimX() * 0.2;

    applyDensityBlock(scene, scene.getDimX() / 2 - swidth / 2, scene.getDimY() - scene.getDimY() / 5 - sheight / 2, swidth, sheight, 0.75f);
    //applyVelocityBlock(scene, scene.getDimX() / 2 - swidth / 2, scene.getDimY() - scene.getDimY() / 5 - sheight / 2, swidth, sheight, Vector3D(0, 0.5, 0));


    FluidSolver solver(scene);

    GUI gui("GraphicsApp");

    double t = TIMESTEP;

    while(!gui.shouldClose()){

        if(gui.getTime() - t < TIMESTEP) continue;
        t += TIMESTEP;

        solver.next();
        gui.display(scene);
    }

    return 0;
}