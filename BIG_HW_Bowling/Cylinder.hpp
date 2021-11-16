#pragma once

#include <vector>

#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Cylinder
{
public:
    Cylinder(float baseRadius, float topRadius, float height, int sectorCount, int stackCount);
    Cylinder();
    ~Cylinder() = default;
    Cylinder(const Cylinder&) = delete;

public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    uint32_t baseIndex, topIndex;
    uint16_t shaderID;

private:
    glm::vec3 xyzPos;
    std::vector<float> Cylinder::buildUnitCircleVertices(int sectorCount);
    void Cylinder::addVertex(glm::vec3 pos);
};