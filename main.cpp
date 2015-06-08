#include <iostream>
#include "GUI/GUI.h"
#include "FluidSolver/FluidSolver.h"


#define TIMESTEP 1.0/60

int main(void)
{

    Scene2D scene(64, 64);


    //initial water
    int height = scene.getDimY() / 4;
    applyDensityBlock(scene, 1, scene.getDimY() - height - 1, scene.getDimX() - 2, height, 0.75f);

    //initial object
    int size = scene.getDimY() / 10 ;
    applyDensityBlock(scene, scene.getDimX() / 2 - size / 2, scene.getDimY() / 3 - size / 2, size, size, 0.75f);

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