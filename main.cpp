#include <iostream>
#include <ctime>
#include "general/Definitions.h"


#ifdef SHOW_GUI
#include "gui/GUI.h"
#endif

#ifdef SIMPLE
#include "lbm/SimpleD2Q9.h"
#endif

#ifdef OPT
#include "lbm/OptD2Q9.h"
#endif

int main(int argc, char** args)
{

    //Get number of iterations from command line
    if(argc < 2) {
        std::cout << "Please specify the number of iterations." << std::endl;
        return -1;
    }

    int numIterations = atoi(args[1]);

#ifdef SIMPLE
    SimpleD2Q9 sim(GRID_SIZE, GRID_SIZE);
    std::string title = "Simple LBM ( 2 Arrays )";
#endif

#ifdef OPT
    OptD2Q9 sim(GRID_SIZE, GRID_SIZE);
    std::string title = "Opt LBM ( 2 Arrays )";
#endif

    std::cout << "Running " << title << std::endl;
    std::cout << numIterations << " Iterations on a " << GRID_SIZE << "x" << GRID_SIZE << " grid." << std::endl;

#ifdef SHOW_GUI
    GUI gui(title);
    VectorGrid vel(GRID_SIZE, GRID_SIZE);
#endif

    sim.init(); //do not measure the initialisation step

    int i = 0;

#ifndef SHOW_GUI
    std::clock_t start;
    start = std::clock();
#endif

    for(i = 0; i < numIterations; ++i) {

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

#ifdef OUTPUT_TO_LOGFILE
    if( freopen("log", "a", stdout) == nullptr) {
        return 0;
    };
#endif


#ifndef SHOW_GUI
    double elapsedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;

    std::cout << "Finished " << title << std::endl;
    std::cout << numIterations << " Iterations on a " << GRID_SIZE << "x" << GRID_SIZE << " grid." << std::endl;
    std::cout << "Elapsed time: " << elapsedTime << std::endl;

    double mlups = numIterations;
    mlups /= elapsedTime;
    mlups *= N;
    mlups /= 1.e6;
    std::cout << "MLUPS: " <<  mlups << std::endl;
    std::cout << std::endl;
#endif
    return 0;
}