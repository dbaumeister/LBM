//
// Created by dbaumeister on 08.06.15.
//

#ifndef GRAPHICS_DEFINITIONS_H
#define GRAPHICS_DEFINITIONS_H

//Control the program by defining either SWAP, BLOCK or SIMPLE.
#define BLOCK

//Show the GUI by defining SHOW_GUI here.
//#define SHOW_GUI

//Define OUTPUT_TO_LOGFILE to redirect std::cout to the file "log"
#define OUTPUT_TO_LOGFILE

//Define BUNDLE to use the bundle data layout
#define BUNDLE

//Defining CAVITY shows another setup on the simple implementation
//#define CAVITY

//Control the problem size with these constants.
static const int NUM_TIME_MEASUREMENTS = 20;
static const int NUM_ITERATIONS = 10;
static const int GRID_SIZE = 512;

static const int N = GRID_SIZE * GRID_SIZE;


#endif //GRAPHICS_DEFINITIONS_H
