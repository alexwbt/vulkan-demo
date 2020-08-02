#include "window.h"

#include <iostream>

GLFWwindow* Window::window;
bool Window::resized = false;
int Window::width = -1;
int Window::height = -1;

void Window::initialize(int width, int height, const char* title)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Tell GLFW to not create an OpenGL context.
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Disable resizing window.

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    Window::width = width;
    Window::height = height;
}

bool Window::update()
{
    glfwPollEvents();
    return !glfwWindowShouldClose(window);
}

void Window::terminate()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    Window::width = width;
    Window::height = height;
    resized = true;
}
