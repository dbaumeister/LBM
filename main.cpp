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
    std::string title = "Simple LBM ( 2 Arrays )";
#endif

#ifdef SWAP
    SwapD2Q9 sim(GRID_SIZE, GRID_SIZE);
    std::string title = "Swap LBM ( 1 Array )";
#endif

#ifdef BLOCK
    BlockD2Q9 sim(GRID_SIZE, GRID_SIZE);
    std::string title = "Block LBM ( 2 Arrays )";
#endif


#ifdef BUNDLE
    title.append(" - Data layout: Bundle");
#endif

#ifdef COLLOPT
    title.append(" - Data layout: CollOpt");
#endif

#ifdef PROPOPT
    title.append(" - Data layout: PropOpt");
#endif


#ifdef SHOW_GUI
    GUI gui(title);
#endif

    sim.init(); //do not measure the initialisation step

    std::clock_t start;
    double elapsedTimes[NUM_TIME_MEASUREMENTS];
#ifdef SHOW_GUI
    VectorGrid vel(GRID_SIZE, GRID_SIZE);
#endif

    int i = 0;
    int j = 0;
    for(; j < NUM_TIME_MEASUREMENTS; ++j) {

#ifdef SHOW_GUI
        if(gui.shouldClose()) {
            std::cout << "---- Aborted by User Input ----" << std::endl;
            break;
        }
#endif

        start = std::clock();
        for(i = 0; i < NUM_ITERATIONS; ++i) {

#ifdef SHOW_GUI
            if(gui.shouldClose()) {
                break;
            }

            sim.getVel(vel);
            gui.display(vel);
#endif
            sim.collide();
            sim.stream();
        }

        elapsedTimes[j] = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    }


#ifdef OUTPUT_TO_LOGFILE
    if( freopen("log", "a", stdout) == nullptr) {
        return 0;
    };
#endif


#ifndef SHOW_GUI
    std::cout << "Finished " << title << std::endl;
    std::cout << j << " * " << NUM_ITERATIONS << " Iterations on a " << GRID_SIZE << "x" << GRID_SIZE << " grid." << std::endl;

    double sum = 0.;
    std::cout << "Elapsed times: ";
    for(int i = 0; i < j; ++i) {
        std::cout << elapsedTimes[i] << "; ";
        sum += elapsedTimes[i];
    }

    std::cout << std::endl;
    double mlups = NUM_TIME_MEASUREMENTS * NUM_ITERATIONS;
    mlups /= 1.e6;
    mlups *= N;
    mlups /= sum;
    std::cout << "MLUPS: " <<  mlups << std::endl;
    std::cout << std::endl;
#endif
    return 0;
}