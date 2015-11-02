#include <iostream>
#include <ctime>
#include "general/Definitions.h"


#ifdef SHOW_GUI
#include "gui/GUI.h"
#endif

#ifdef SIMPLE
#include "lbm/SimpleD2Q9.h"
#endif

#ifdef SWAP
#include "lbm/SwapD2Q9.h"
#endif

#ifdef BLOCK
#include "lbm/BlockD2Q9.h"
#endif

int main(void)
{

#ifdef SIMPLE
    SimpleD2Q9 sim(GRID_SIZE, GRID_SIZE);
    std::string title = "Simple LBM ( 2 - Array )";
#endif

#ifdef SWAP
    SwapD2Q9 sim(GRID_SIZE, GRID_SIZE);
    std::string title = "Swap LBM ( 1 - Array )";
#endif

#ifdef BLOCK
    BlockD2Q9 sim(GRID_SIZE, GRID_SIZE);
    std::string title = "Block LBM ( 2 - Array )";
#endif

#ifdef SHOW_GUI
    GUI gui(title);
#endif

#ifdef CHECK
    sim.init();
    if(!sim.verify()) {
        return 0;
    }
#endif

    sim.init(); //do not measure the initialisation step

    std::clock_t start;
    double elapsedTime;

#ifdef SHOW_GUI
    VectorGrid vel(GRID_SIZE, GRID_SIZE);
#endif

#ifdef OUTPUT_TO_LOGFILE
    freopen("log", "a", stdout);
#endif

    start = std::clock();
    int i = 0;
    for(; i < NUM_ITERATIONS; ++i) {

#ifdef SHOW_GUI
        if(gui.shouldClose()) {
            std::cout << "---- Aborted by User Input ----" << std::endl;
            break;
        }

        sim.getVel(vel);
        gui.display(vel);
#endif

        sim.collide();
        sim.stream();
    }

    elapsedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << "Finished " << title << std::endl;
    std::cout << i << " Iterations on a " << GRID_SIZE << "x" << GRID_SIZE << " grid." << std::endl;
    std::cout << "Elapsed time: " << elapsedTime << std::endl;
    std::cout << std::endl;
    return 0;
}