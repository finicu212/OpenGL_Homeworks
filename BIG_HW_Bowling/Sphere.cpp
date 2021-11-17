#include "Sphere.hpp"
#include "shader.hpp"

#include <math.h>

#define PI 3.14159265359

void Sphere::loadShader()
{
    shaderID = LoadShaders("shaders/Sphere.vertexshader", "shaders/Sphere.fragmentshader");
}

Sphere::Sphere(float radius) : Sphere(radius, radius * 24.0, radius * 24.0) {}

Sphere::Sphere(float radius, unsigned int sectors, unsigned int stacks)
{
    vertices.reserve(stacks * sectors);
    indices.reserve(stacks * sectors * 6);
    const float STEP_STACK = PI / stacks;
    const float STEP_SECTOR = 2 * PI / sectors;

    float x, y, z, xy;                              // vertex position

    float sectorAngle, stackAngle;

    for (int i = 0; i <= stacks; ++i)
    {
        stackAngle = PI / 2 - i * STEP_STACK;        // starting from pi/2 to -pi/2
        xy = radius * std::cos(stackAngle);             // r * cos(u)
        z = radius * std::sin(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectors; ++j)
        {
            sectorAngle = j * STEP_SECTOR;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * std::cos(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * std::sin(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(glm::vec3{x, y, z});
        }
    }

    int k1, k2;
    for (int i = 0; i < stacks; ++i)
    {
        k1 = i * (sectors + 1);     // beginning of current stack
        k2 = k1 + sectors + 1;      // beginning of next stack

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}