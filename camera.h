#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();

    glm::vec3 position, front, right, up;
    float yaw, pitch, fov;

    glm::mat4 viewMatrix;

    void update();
};
