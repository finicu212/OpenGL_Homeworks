#pragma once

#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 viewDirection;
    glm::vec3 up;
    glm::vec3 right;

public:
    Camera();
    Camera(glm::vec3 position);
    Camera(glm::vec3 position, glm::vec3 viewDirection, glm::vec3 cameraUp);
    ~Camera();
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

