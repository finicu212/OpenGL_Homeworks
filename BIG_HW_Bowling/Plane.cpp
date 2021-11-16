#include "Plane.hpp"

/*
* rectangular thin plane
* yLevel           - (float) z coordinate of plane
*/
Plane::Plane(unsigned int xSegments, unsigned int zSegments, float yLevel)
{
    shaderID = LoadShaders("shaders/Plane.vertexshader", "shaders/Plane.fragmentshader");

    bool oddRow = false;

    float dX = 1.0f / xSegments;
    float dY = 1.0f / zSegments;

    for (int y = 0; y <= zSegments; ++y)
    {
        for (int x = 0; x <= xSegments; ++x)
        {
            vertices.push_back(glm::vec3(dX * x * 2.0f - 1.0f, dY * y * 2.0f - 1.0f, yLevel));
        }
    }

    for (int y = 0; y < zSegments; ++y)
    {
        if (!oddRow)
        {
            for (int x = 0; x <= xSegments; ++x)
            {
                indices.push_back(y * (xSegments + 1) + x);
                indices.push_back((y + 1) * (xSegments + 1) + x);
            }
        }
        else
        {
            for (int x = xSegments; x >= 0; --x)
            {
                indices.push_back((y + 1) * (xSegments + 1) + x);
                indices.push_back(y * (xSegments + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
}