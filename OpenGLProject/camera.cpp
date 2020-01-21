#include "camera.hpp"

// Constructors / Destructor

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp) {
    this->position = position;
    this->forward = direction;
    this->up = worldUp;
    this->worldUp = worldUp;
    this->right = glm::vec3(0.0f);

    this->viewMatrix = glm::mat4(1.0f);

    this->movementSpeed = 3.0f;
    this->sensitivity = 50.0f;

    this->pitch = 0.0f;
    this->yaw = -90.0f;
    this->roll = 0.0f;
    
    this->updateCameraVectors();
}

Camera::~Camera() {}

// Private functions

void Camera::updateCameraVectors() {
    // Euler rotations
    this->forward.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->forward.y = sin(glm::radians(this->pitch));
    this->forward.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->forward = glm::normalize(this->forward);
    this->right = glm::normalize(glm::cross(this->forward, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->forward));
}

// Public functions
void Camera::move(const float &deltaTime, const int direction) {
    switch (direction) {
        case FORWARD:
            this->position += this->forward * this->movementSpeed * deltaTime;
            break;
        case BACKWARD:
            this->position -= this->forward * this->movementSpeed * deltaTime;
            break;
        case LEFT:
            this->position -= this->right * this->movementSpeed * deltaTime;
            break;
        case RIGHT:
            this->position += this->right * this->movementSpeed * deltaTime;
            break;
        default:
            break;
    };
}

void Camera::rotate(const float &deltaTime, const double &offsetX, const double &offsetY) {
    this->pitch += static_cast<GLfloat>(offsetY) * this->sensitivity * deltaTime;
    this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity * deltaTime;

    if (this->pitch > 80.f)
        this->pitch = 80.f;
    else if (this->pitch < -80.f)
        this->pitch = -80.f;

    if (this->yaw == 360.f || this->yaw == -360.f)
        this->yaw = 0.f;
}

void Camera::updateInput(const float &deltaTime, const int direction, const double &offsetX, const double &offsetY) {
    this->rotate(deltaTime, offsetX, offsetY);
}

const glm::mat4 Camera::getViewMatrix() {
    this->updateCameraVectors();

    this->viewMatrix = glm::lookAt(this->position, this->position + this->forward, this->up);

    return this->viewMatrix;
}

const glm::vec3 Camera::getPosition() { return this->position; }
