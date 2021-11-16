#include "Camera.hpp"

Camera::Camera(glm::vec3 position)
{
    this->position = position;
    this->viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->right = glm::cross(viewDirection, up);
}

Camera::Camera()
{
    this->position = glm::vec3(0.0f, 0.0f, 100.0f);
    this->viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->right = glm::cross(viewDirection, up);
}

Camera::Camera(glm::vec3 position, glm::vec3 viewDirection, glm::vec3 up)
{
    this->position = position;
    this->viewDirection = viewDirection;
    this->up = up;
    this->right = glm::cross(viewDirection, up);
}

Camera::~Camera()
{
}

void Camera::rotateOx(float angle)
{
    viewDirection = glm::vec3(glm::normalize(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(viewDirection, 1.0f)));
    up = glm::normalize(glm::cross(viewDirection, right));
}

void Camera::rotateOy(float angle)
{
    viewDirection = glm::vec3(glm::normalize(glm::rotate(glm::mat4(1.0f), angle, up) * glm::vec4(viewDirection, 1.0f)));
    up = glm::normalize(glm::cross(viewDirection, right));
}

void Camera::translateUp(float speed)
{
    position = position + speed * up;
}

void Camera::translateDown(float speed)
{
    position = position - speed * up;
}

void Camera::translateFront(float speed)
{
    position = position + speed * viewDirection;
}

void Camera::translateBack(float speed)
{
    position = position - speed * viewDirection;
}

void Camera::translateRight(float speed)
{
    position = position + speed * right;
}

void Camera::translateLeft(float speed)
{
    position = position - speed * right;
}
/*
glm::vec3 Camera::getPosition()
{
    return position;
}

glm::vec3 Camera::getViewDirection()
{
    return viewDirection;
}

glm::vec3 Camera::getUp()
{
    return up;
}

void Camera::setPosition(glm::vec3 position)
{
    this->position = position;
}

void Camera::setUp(glm::vec3 up)
{
    this->up = up;
}

void Camera::setViewDirection(glm::vec3 viewDirection)
{
    this->viewDirection = viewDirection;
}
*/
