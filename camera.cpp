#include "camera.h"

#include <gtc/matrix_transform.hpp>

Camera::Camera()
{
    position = glm::vec3(0);

    yaw = -90.0f;
    pitch = 0.0f;

    fov = 80.0f;

    update();
}

void Camera::update()
{
    front = glm::normalize(glm::vec3(
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::sin(glm::radians(pitch)),
        glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
    ));
    right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    up = glm::normalize(glm::cross(right, front));

    viewMatrix = glm::lookAt(position, position + front, up);
}
