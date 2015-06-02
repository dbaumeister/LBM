#include "GUI/GUI.h"
#include "FluidSolver/FluidSolver.h"


int main(void)
{

    Scene2D scene(128, 128);
    FluidSolver solver(scene);

    GUI gui("GraphicsApp");

    while(!gui.shouldClose()){
        solver.next();
        gui.display(scene);
    }

    return 0;
}