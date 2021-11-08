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
    Sphere(const Sphere&) = delete;

public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
};