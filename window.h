#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
    static GLFWwindow* window;
    static bool resized;
    static int width, height;

    static void initWindow(int width, int height, const char* title);
private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};
