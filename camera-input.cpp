#include "camera-input.h"

CameraInput::CameraInput(GLFWwindow* window, Camera& camera) : window{ window }, camera{ camera }
{
    mouseX = 0;
    mouseY = 0;
}

void CameraInput::update(float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += deltaTime * cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= deltaTime * cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += deltaTime * cameraSpeed * camera.right;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= deltaTime * cameraSpeed * camera.right;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.position += deltaTime * cameraSpeed * glm::vec3(0, 1, 0);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.position -= deltaTime * cameraSpeed * glm::vec3(0, 1, 0);

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (mouseX != 0 || mouseY != 0)
    {
        camera.yaw += (x - mouseX) * sensitivity;
        camera.pitch += (mouseY - y) * sensitivity;

        if (camera.pitch > 89.0f)
            camera.pitch = 89.0f;
        if (camera.pitch < -89.0f)
            camera.pitch = -89.0f;
    }
    mouseX = x;
    mouseY = y;

    camera.update();
}
