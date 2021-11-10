#pragma once

#include <vector>

#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Cylinder
{
public:
    // base radius, top radius, height, sectors, stacks
    Cylinder(float baseRadius, float topRadius, float height, int sectorCount, int stackCount);
    ~Cylinder() = default;
    Cylinder(const Cylinder&) = delete;

public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    unsigned int baseIndex, topIndex;

private:
    std::vector<float> Cylinder::buildUnitCircleVertices(int sectorCount);
};

/*
std::vector<float> Cylinder::getUnitCircleVertices()
{
    const float PI = 3.1415926f;
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    std::vector<float> unitCircleVertices;
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }
    return unitCircleVertices;
}

// generate vertices for a cylinder
void Cylinder::buildVerticesSmooth()
{
    // clear memory of prev arrays
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);

    // get unit circle vectors on XY-plane
    std::vector<float> unitVertices = getUnitCircleVertices();

    // put side vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
        float t = 1.0f - i;                              // vertical tex coord; 1 to 0

        for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k + 1];
            float uz = unitVertices[k + 2];
            // position vector
            vertices.push_back(ux * radius);             // vx
            vertices.push_back(uy * radius);             // vy
            vertices.push_back(h);                       // vz
            // normal vector
            normals.push_back(ux);                       // nx
            normals.push_back(uy);                       // ny
            normals.push_back(uz);                       // nz
            // texture coordinate
            texCoords.push_back((float)j / sectorCount); // s
            texCoords.push_back(t);                      // t
        }
    }

    // the starting index for the base/top surface
    //NOTE: it is used for generating indices later
    int baseCenterIndex = (int)vertices.size() / 3;
    int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

    // put base and top vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
        float nz = -1 + i * 2;                           // z value of normal; -1 to 1

        // center point
        vertices.push_back(0);     vertices.push_back(0);     vertices.push_back(h);
        normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
        texCoords.push_back(0.5f); texCoords.push_back(0.5f);

        for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k + 1];
            // position vector
            vertices.push_back(ux * radius);             // vx
            vertices.push_back(uy * radius);             // vy
            vertices.push_back(h);                       // vz
            // normal vector
            normals.push_back(0);                        // nx
            normals.push_back(0);                        // ny
            normals.push_back(nz);                       // nz
            // texture coordinate
            texCoords.push_back(-ux * 0.5f + 0.5f);      // s
            texCoords.push_back(-uy * 0.5f + 0.5f);      // t
        }
    }
}
*/