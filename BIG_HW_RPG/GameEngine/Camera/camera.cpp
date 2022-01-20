#include "camera.h"

#define PI_OVER_TWO 1.57079632679

Camera::Camera(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
	this->rotationOx = 0.0f;
	this->rotationOy = -90.0f;
}

Camera::Camera()
{
	this ->cameraPosition = glm::vec3(0.0f, 0.0f, 100.0f);
	this ->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this ->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
	this->rotationOx = 0.0f;
	this->rotationOy = -90.0f;
}

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = cameraViewDirection;
	this->cameraUp = cameraUp;
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

Camera::~Camera()
{
}

void Camera::keyboardMoveFront(float cameraSpeed)
{
	cameraPosition += cameraViewDirection * cameraSpeed;
	//cameraPosition.y = 0.0f;
}

void Camera::keyboardMoveBack(float cameraSpeed)
{
	cameraPosition -= cameraViewDirection * cameraSpeed;
	//cameraPosition.y = 0.0f;
}

void Camera::keyboardMoveLeft(float cameraSpeed)
{
	cameraPosition -= glm::normalize(glm::cross(cameraViewDirection, cameraUp)) * cameraSpeed;
	//cameraPosition.y = 0.0f;
}

void Camera::keyboardMoveRight(float cameraSpeed)
{
	cameraPosition += glm::normalize(glm::cross(cameraViewDirection, cameraUp)) * cameraSpeed;
	//cameraPosition.y = 0.0f;
}

void Camera::keyboardMoveUp(float cameraSpeed)
{
	cameraPosition += cameraUp * cameraSpeed;
	//cameraPosition.y = 0.0f;
}

void Camera::keyboardMoveDown(float cameraSpeed)
{
	cameraPosition -= cameraUp * cameraSpeed;
	//cameraPosition.y = 0.0f;
}

void Camera::updateRotateVecs()
{
    //viewDirection = glm::vec3(glm::normalize(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(viewDirection, 1.0f)));

    cameraViewDirection.x = -sinf(yaw) * cosf(pitch);
	cameraViewDirection.y = -sinf(pitch);
	cameraViewDirection.z = -cos(yaw) * cos(pitch);

    cameraRight.x = -cos(yaw);
	cameraRight.y = 0.0;
	cameraRight.z = sin(yaw);
    cameraUp = glm::cross(cameraViewDirection, cameraRight);
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

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(cameraPosition, cameraPosition + cameraViewDirection, cameraUp);
}

glm::vec3 Camera::getCameraPosition()
{
	return cameraPosition;
}

glm::vec3 Camera::getCameraViewDirection()
{
	return cameraViewDirection;
}

glm::vec3 Camera::getCameraUp()
{
	return cameraUp;
}


