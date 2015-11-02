//
// Created by dbaumeister on 02.06.15.
//

#include <iostream>
#include "GUI.h"


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


bool GUI::shouldClose() {
    glfwPollEvents();
    return glfwWindowShouldClose(window) != 0;
}

void GUI::display(const VectorGrid& vel){
    renderWindow(vel);
    glfwSwapBuffers(window);
}

// Mapping von
// 0/128-1------128-1/128-1
//  |                |
//  |                |
//  |                |
//  |                |
// 0/0 -------- 128-1/0

//auf
// -1/1 ----------- 1/1
//   |               |
//   |               |
//   |               |
//   |               |
// -1/-1------------1/-1

inline double scaleX(double i, int dimX){
    return IMAGESCALE * (i / (0.5 * (dimX)) - 1.);
}

inline double scaleY(double j, int dimY){
    return IMAGESCALE * (j / (0.5 * (dimY)) - 1.);
}

void draw(const VectorGrid& grid) {

    glBegin(GL_LINES);
    double x, y, z = 0.;
    double velScale = 2.;
    glColor3f(0.9f, 0.f, 0.f);

    int dimX = grid.getDimX();
    int dimY = grid.getDimY();

    for(int i = 0; i < dimX - 1; ++i){
        for(int j = 0; j < dimY - 1; ++j){

            x = scaleX(i + 0.5, dimX);
            y = scaleY(j + 0.5, dimY);

            glVertex3f((float)x, (float)y, (float)z);

            double velX = grid.get(i, j).x;
            double velY = grid.get(i, j).y;
            x = scaleX(i + 0.5, dimX) +  velX * velScale * IMAGESCALE / (dimX - 1.);
            y = scaleY(j + 0.5, dimY) +  velY * velScale * IMAGESCALE / (dimY - 1.);

            glVertex3f((float)x, (float)y, (float)z);

        }
    }
    glEnd();
}


void GUI::renderWindow(const VectorGrid& vel) {
    // Read current window size and set viewport accordingly
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Clear background
    glClear(GL_COLOR_BUFFER_BIT);


    //update camera settings if the user resized the window
    float ratio;
    ratio = width / (float) height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1., 1., 1., -1.);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //draw here
    draw(vel);
}

void GUI::initWindow() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(defaultWith, defaultHeight, title.c_str(), NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    glClearColor(1.f,1.f,1.f,1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}

void GUI::closeWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
    isWindowDestroyed = true;
}
