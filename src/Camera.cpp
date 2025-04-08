#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp)
{
    this->position = position;
    this->worldUp = worldUp;

    updateVectors();
}

void Camera::processMouseInput(float x, float y)
{
    if (firstMouseEnter) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouseEnter = false;
    }

    float xOffset = x - lastMouseX;
    float yOffset = y - lastMouseY;
    lastMouseX = x;
    lastMouseY = y;

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;
    yaw += xOffset;
    pitch -= yOffset;


    if(constrainPitch){
        if (pitch >= 89) pitch = 89;
        if (pitch <= -89) pitch = -89;
    }

    updateVectors();

}

void Camera::processKeyboardInput(int key, float deltaTime)
{
    updateVectors();

    if(key == GLFW_KEY_W){
        position += forward * moveSpeed * deltaTime;
    }

    if(key == GLFW_KEY_S){
        position -= forward * moveSpeed * deltaTime;
    }

    if(key == GLFW_KEY_A){
        position -= right * moveSpeed * deltaTime;
    }

    if (key == GLFW_KEY_D) {
        position += right * moveSpeed * deltaTime;
    }

}

void Camera::updateVectors()
{
    
    glm::vec3 direction;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    forward = glm::normalize(direction);

    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));

}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + forward, up);
}





