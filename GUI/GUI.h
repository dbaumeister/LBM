//
// Created by dbaumeister on 02.06.15.
//

#ifndef GRAPHICS_GUI_H
#define GRAPHICS_GUI_H

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

class GUI {
public:

    GUI(std::string title, int defaultWidth = 200, int defaultHeight = 200)
            : title(title), defaultWith(defaultWidth), defaultHeight(defaultHeight){
        initWindow();
        isWindowDestroyed = false;
    }

    ~GUI(){
        if(!isWindowDestroyed)
            closeWindow();
    }

    void display();

private:
    std::string title;
    int defaultWith, defaultHeight;
    GLFWwindow* window;
    bool isWindowDestroyed;

    void initWindow();
    void closeWindow();

};



#endif //GRAPHICS_GUI_H
