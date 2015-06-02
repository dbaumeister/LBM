#include "GUI/GUI.h"
#include "FluidSolver/FluidSolver.h"


int main(void)
{

    GUI gui("GraphicsApp");
    Scene2D scene(128, 128);
    FluidSolver solver(scene);

    //TODO open  in a new thread -> should not stop others from executing
    while(!gui.shouldClose()){
        solver.next();
        gui.display(scene);
    }


    return 0;
}