#pragma once

#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Camera
{
private:
    float pitch, yaw; // vars we use to rotate
    void updateRotateVecs();

public:
    glm::vec3 position;
    glm::vec3 viewDirection;
    glm::vec3 up;
    glm::vec3 right;
    const float FOV = 65.0f;

public:
    Camera();
    Camera(glm::vec3 position);
    Camera(glm::vec3 position, glm::vec3 viewDirection, glm::vec3 cameraUp);
    ~Camera() = default;
    /*
    glm::vec3 getCameraPosition();
    glm::vec3 getCameraViewDirection();
    glm::vec3 getCameraUp();

    void setPosition(glm::vec3 cameraPosition);
    void setViewDirection(glm::vec3 cameraViewDirection);
    void setUp(glm::vec3 cameraUp);
    */
    void rotateOx(float angle);
    void rotateOy(float angle);

    void translateFront(float speed);
    void translateBack(float speed);
    void translateUp(float speed);
    void translateDown(float speed);
    void translateLeft(float speed);
    void translateRight(float speed);
};

