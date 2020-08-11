#pragma once

#include "camera.h"

#include <GLFW/glfw3.h>

class CameraInput
{
public:
    CameraInput(GLFWwindow* window, Camera& camera);

	void update(float deltaTime);
private:
    GLFWwindow* window;
    Camera& camera;
    
    double mouseX, mouseY;

    float cameraSpeed = 1.0f;
    float sensitivity = 0.1f;
};
