#pragma once

#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
    static GLFWwindow* window;
    static bool resized;
    static int width, height;
    static std::string title;

    static void initialize(int width, int height, std::string title);
    static bool update();
    static void terminate();
    static void setTitle(std::string title);
private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};
