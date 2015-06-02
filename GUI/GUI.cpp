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

void GUI::renderWindow(Scene2D& scene) {
    // Read current window size and set viewport accordingly
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Set background color
    glClear(GL_COLOR_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_POINT);
    glPointSize(10);
    glColor3f(1.f, 0.f, 0.f);
    glVertex2f(0.6f, 0.4f);
    glEnd();
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
}

void GUI::closeWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
    isWindowDestroyed = true;
}

