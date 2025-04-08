#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0));

    float moveSpeed = 1;
    float mouseSensitivity = 0.2f;
    bool constrainPitch = true;
    bool unlocked = false;

    void processMouseInput(float xOffset, float yOffset);
    void processKeyboardInput(int key, float deltaTime);
    glm::mat4 getViewMatrix();
    glm::vec3 position = glm::vec3(0, 0, 0);


private:
    glm::vec3 worldUp;
    glm::vec3 up;
    glm::vec3 forward;
    glm::vec3 right;
    float yaw = -90, pitch = -45;
    float lastMouseX, lastMouseY;
    bool firstMouseEnter = true;

    
    void updateVectors();


};

