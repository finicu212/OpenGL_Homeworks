#include "Camera.hpp"
#include <math.h>
#include <iostream>

#define PI_OVER_TWO 1.57079632679

Camera::Camera(glm::vec3 position, glm::vec3 viewDirection, glm::vec3 up)
{
    this->position = position;
    this->viewDirection = viewDirection;
    this->up = up;
    this->right = glm::cross(viewDirection, up);
    this->pitch = 0.f;
    this->yaw = 0.f;
}

Camera::Camera(glm::vec3 position) : Camera(position, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {}

Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {}

void Camera::updateRotateVecs()
{
    //viewDirection = glm::vec3(glm::normalize(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(viewDirection, 1.0f)));

    viewDirection.x = -sinf(yaw) * cosf(pitch);
    viewDirection.y = -sinf(pitch);
    viewDirection.z = -cos(yaw) * cos(pitch);

    right.x = -cos(yaw);
    right.y = 0.0;
    right.z = sin(yaw);
    up = glm::cross(viewDirection, right);
}

void Camera::rotateOx(float angle)
{
    pitch += angle;

    if (pitch > PI_OVER_TWO) {
        pitch = PI_OVER_TWO - 0.0001f;
    }
    else if (pitch < -PI_OVER_TWO) {
        pitch = -PI_OVER_TWO + 0.0001f;
    }

    updateRotateVecs();
}

void Camera::rotateOy(float angle)
{
    yaw += angle;

    updateRotateVecs();
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
