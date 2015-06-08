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

void GUI::display(Scene2D& scene){ //todo: draw scene once
    renderWindow(scene);
    glfwSwapBuffers(window);
}

// Mapping von
// 0/0 -------- 128-1/0
//  |                |
//  |                |
//  |                |
//  |                |
// 0/128-1------128-1/128-1

//auf
// -1/1 ----------- 1/1
//   |               |
//   |               |
//   |               |
//   |               |
// -1/-1------------1/-1

inline float scaleX(int i, int dimX){
    return IMAGESCALE * (i / (0.5f * (dimX)) - 1.f);
}

inline float scaleY(int j, int dimY){
    return IMAGESCALE * (1.f - j / (0.5f * (dimY)));
}


void drawDensity(Scene2D& scene) {
    glBegin(GL_QUADS);

    Real c, x, y, z = 0;
    for(int i = 0; i < scene.getDimX(); ++i){
        for(int j = 0; j < scene.getDimY(); ++j){

            c = std::min(1.f, scene.density(i, j));
            glColor3f(1.f - c, 1.f - c, 1.f - c);

            x = scaleX(i, scene.getDimX());
            y = scaleY(j, scene.getDimY());
            glVertex3f(x, y, z);

            x = scaleX(i, scene.getDimX());
            y = scaleY(j + 1, scene.getDimY());
            glVertex3f(x, y, z);

            x = scaleX(i + 1, scene.getDimX());
            y = scaleY(j + 1, scene.getDimY());
            glVertex3f(x, y, z);

            x = scaleX(i + 1, scene.getDimX());
            y = scaleY(j, scene.getDimY());
            glVertex3f(x, y, z);
        }
    }
    glEnd();
}

void drawVelocity(Scene2D& scene) {

    glBegin(GL_LINES);
    Real x, y, z = 0.f;
    Real velScale = 2.f;
    glColor3f(0.9f, 0.f, 0.f);

    for(int i = 0; i < scene.getDimX() - 1; ++i){
        for(int j = 0; j < scene.getDimY() - 1; ++j){

            x = scaleX(i, scene.getDimX());
            y = scaleY(j, scene.getDimY());

            glVertex3f(x, y, z);

            Real velX = (scene.vel(i, j).x + scene.vel(i + 1, j).x) / 2.f;
            Real velY = (scene.vel(i, j).y + scene.vel(i, j + 1).y) / 2.f;
            x = scaleX(i, scene.getDimX()) +  velX * velScale * IMAGESCALE / (scene.getDimX() - 1.f);
            y = scaleY(j, scene.getDimY()) +  velY * velScale * IMAGESCALE / (scene.getDimY() - 1.f);

            glVertex3f(x, y, z);

        }
    }
    glEnd();
}


void GUI::renderWindow(Scene2D& scene) {
    // Read current window size and set viewport accordingly
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Set background color
    glClear(GL_COLOR_BUFFER_BIT);


    //update camera settings if the user resized the window
    float ratio;
    ratio = width / (float) height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawDensity(scene);
    drawVelocity(scene);
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
}

void GUI::closeWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
    isWindowDestroyed = true;
}

double GUI::getTime() {
    return glfwGetTime();
}
