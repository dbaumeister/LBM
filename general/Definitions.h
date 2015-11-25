//
// Created by dbaumeister on 08.06.15.
//

#ifndef GRAPHICS_DEFINITIONS_H
#define GRAPHICS_DEFINITIONS_H

//Control the program by defining either OPT or SIMPLE.
#define OPT

//Control the problem size with these constants.
static const int GRID_SIZE = 64;
static const int N = GRID_SIZE * GRID_SIZE;

//Show the GUI by defining SHOW_GUI here.
#define SHOW_GUI

//Defining CAVITY shows another setup on the simple and block implementation
#define CAVITY

//Define OUTPUT_TO_LOGFILE to redirect std::cout to the file "log"
#define OUTPUT_TO_LOGFILE



#endif //GRAPHICS_DEFINITIONS_H



