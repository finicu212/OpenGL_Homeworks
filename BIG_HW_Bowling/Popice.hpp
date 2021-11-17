#pragma once

#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include <vector>
#include <map>

#include "Cylinder.hpp"
#include "shader.hpp"

class Popice
{
public:
    Popice(glm::vec3 xyzPos, float popicaSpace);
    ~Popice() = default;
    Popice(const Popice&) = delete;
    Popice() = delete;
    void loadShader();

public:
    uint32_t numTriangles;
    uint16_t shaderID;
    glm::mat4 model;
    Cylinder defaultPopic;
    std::vector<glm::vec3> popicaMapPositions;
};