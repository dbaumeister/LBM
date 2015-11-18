//
// Created by dbaumeister on 08.06.15.
//

#ifndef GRAPHICS_DEFINITIONS_H
#define GRAPHICS_DEFINITIONS_H

//Control the program by defining either SWAP, BLOCK or SIMPLE.
#define BLOCK

//Define COLLOPT, PROPOPT or BUNDLE to choose the data layout
#define PROPOPT

//Defining CAVITY shows another setup on the simple and block implementation
//#define CAVITY

//Control the problem size with these constants.
static const int NUM_TIME_MEASUREMENTS = 1;
static const int NUM_ITERATIONS = 10000;
static const int GRID_SIZE = 256;

static const int N = GRID_SIZE * GRID_SIZE;

//Show the GUI by defining SHOW_GUI here.
//#define SHOW_GUI

//Define OUTPUT_TO_LOGFILE to redirect std::cout to the file "log"
#define OUTPUT_TO_LOGFILE



#endif //GRAPHICS_DEFINITIONS_H



