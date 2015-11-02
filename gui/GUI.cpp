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

inline float scaleX(Real i, int dimX){
    return IMAGESCALE * (i / (0.5f * (dimX)) - 1.f);
}

inline float scaleY(Real j, int dimY){
    return IMAGESCALE * (j / (0.5f * (dimY)) - 1.f);
}


void draw(const RealGrid& grid) {
    glBegin(GL_QUADS);

    Real c, x, y, z = 0;
    int dimX = grid.getDimX();
    int dimY = grid.getDimY();

    for(int i = 0; i < dimX; ++i){
        for(int j = 0; j < dimY; ++j){

            c = std::min(1.f, grid.get(i, j));
            glColor3f(1.f - c, 1.f - c, 1.f - c);

            x = scaleX(i, dimX);
            y = scaleY(j, dimY);
            glVertex3f(x, y, z);

            x = scaleX(i, dimX);
            y = scaleY(j + 1, dimY);
            glVertex3f(x, y, z);

            x = scaleX(i + 1, dimX);
            y = scaleY(j + 1, dimY);
            glVertex3f(x, y, z);

            x = scaleX(i + 1, dimX);
            y = scaleY(j, dimY);
            glVertex3f(x, y, z);
        }
    }
    glEnd();
}

void draw(const VectorGrid& grid) {

    glBegin(GL_LINES);
    Real x, y, z = 0.f;
    Real velScale = 2.f;
    glColor3f(0.9f, 0.f, 0.f);

    int dimX = grid.getDimX();
    int dimY = grid.getDimY();

    for(int i = 0; i < dimX - 1; ++i){
        for(int j = 0; j < dimY - 1; ++j){

            x = scaleX(i + 0.5f, dimX);
            y = scaleY(j + 0.5f, dimY);

            glVertex3f(x, y, z);

            Real velX = grid.get(i, j).x;
            Real velY = grid.get(i, j).y;
            x = scaleX(i + 0.5f, dimX) +  velX * velScale * IMAGESCALE / (dimX - 1.f);
            y = scaleY(j + 0.5f, dimY) +  velY * velScale * IMAGESCALE / (dimY - 1.f);

            glVertex3f(x, y, z);

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
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
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
