#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

enum Direction {
    FORWARD = 0,
    BACKWARD = 1,
    LEFT = 2,
    RIGHT = 3
};

class Camera {

private:
    glm::mat4 viewMatrix;

    GLfloat movementSpeed;
    GLfloat sensitivity;

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat pitch;
    GLfloat yaw;
    GLfloat roll;

    void updateCameraVectors();

public:
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp);
    ~Camera();

    // Functions
    void move(const float &deltaTime, const int direction);
    void rotate(const float &deltaTime, const double &offsetX, const double &offsetY);
    void updateInput(const float &deltaTime, const int direction, const double &offsetX, const double &offsetY);

    // Getters
    const glm::mat4 getViewMatrix();
    const glm::vec3 getPosition();

};
