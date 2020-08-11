#include "window.h"

#include <stdexcept>

GLFWwindow* Window::window = nullptr;
bool Window::resized = false;
int Window::width = -1;
int Window::height = -1;
std::string Window::title;

void Window::initialize(int width, int height, std::string title)
{
    if (window != nullptr)
        throw std::runtime_error("Multiple window initialization disabled.");

    if (glfwInit() != GLFW_TRUE)
        throw std::runtime_error("Failed to initialize GLFW.");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Tell GLFW to not create an OpenGL context.
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Disable resizing window.

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Window::width = width;
    Window::height = height;
    Window::title = title;
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

void Window::setTitle(std::string title)
{
    glfwSetWindowTitle(window, title.c_str());
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    Window::width = width;
    Window::height = height;
    resized = true;
}
