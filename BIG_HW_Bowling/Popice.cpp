#include "Popice.hpp"

#include <iostream>

void Popice::loadShader()
{
    shaderID = LoadShaders("shaders/Popice.vertexshader", "shaders/Popice.fragmentshader");
}

Popice::Popice(glm::vec3 xyzPos, float popicaSpace)
{
    model = glm::mat4(1.0f);
    const float SCALE_FACTOR = 12.0f, POPICA_RADIUS = 0.01f;
    defaultPopic = Cylinder(POPICA_RADIUS, POPICA_RADIUS / 2, 0.1f, 8, 6);

    // 4 popics rows :D
    for (uint16_t i = 0; i < 4; i++)
    {
        float zOffsetPos = -i * POPICA_RADIUS * SCALE_FACTOR;
        // popic number
        for (uint16_t j = 0; j < i + 1; j++)
        {
            
            glm::vec3 relativePos(i * (POPICA_RADIUS / 2 + popicaSpace), 0.0f, zOffsetPos + j * SCALE_FACTOR * POPICA_RADIUS * 2);
            std::cout << relativePos.x << ", " << relativePos.y << ", " << relativePos.z << "\n";
            relativePos += xyzPos; // make relative to world

            //std::cout << relativePos.x << ", " << relativePos.y << ", " << relativePos.z << "\n";
            popicaMapPositions.push_back(relativePos);
        }
    }

    numTriangles = defaultPopic.indices.size() * 10;
}