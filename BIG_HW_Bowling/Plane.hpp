#pragma once

#include <vector>
#include "shader.hpp"

#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Plane
{
public:
    Plane(unsigned int xSegments, unsigned int zSegments, float yLevel);
    ~Plane() = default;
    Plane(const Plane&) = delete;

public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    uint16_t shaderID;
};