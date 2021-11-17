#pragma once

#include <vector>

#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Sphere
{
public:
    Sphere(float radius);
    Sphere(float radius, unsigned int sectors, unsigned int stacks);
    ~Sphere() = default;
    Sphere(const Sphere&) = delete;
    void loadShader();

public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    uint16_t shaderID;
    glm::mat4 model;
};