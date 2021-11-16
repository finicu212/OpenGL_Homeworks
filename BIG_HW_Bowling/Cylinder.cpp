#include "Cylinder.hpp"

std::vector<float> Cylinder::buildUnitCircleVertices(int sectorCount)
{
    std::vector<float> unitCircleVertices;
    const float PI = acos(-1);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }

    return unitCircleVertices;
}

void Cylinder::addVertex(glm::vec3 pos)
{
    vertices.push_back(pos + xyzPos);
}

Cylinder::Cylinder() : Cylinder(0.01f, 0.003f, 0.1f, 8, 6) {}

Cylinder::Cylinder(float baseRadius, float topRadius, float height, int sectorCount, int stackCount)
{
    this->xyzPos = xyzPos;
    std::vector<float> unitCircleVertices = buildUnitCircleVertices(sectorCount);
    float x, y, z;
    float radius;

    for (int i = 0; i <= stackCount; ++i)
    {
        z = -(height * 0.5f) + (float)i / stackCount * height;
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);
        float t = 1.0f - (float)i / stackCount;

        for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            x = unitCircleVertices[k];
            y = unitCircleVertices[k + 1];
            addVertex(glm::vec3(x * radius, y * radius, z));
        }
    }

    unsigned int baseVertexIndex = (unsigned int)vertices.size();

    // put vertices of base of cylinder
    z = -height * 0.5f;
    addVertex(glm::vec3(0, 0, z));

    for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j + 1];
        addVertex(glm::vec3(x * baseRadius, y * baseRadius, z));
    }

    // remember where the base vertices start
    unsigned int topVertexIndex = (unsigned int)vertices.size();

    // put vertices of top of cylinder
    z = height * 0.5f;
    addVertex(glm::vec3(0, 0, z));

    for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j + 1];
        addVertex(glm::vec3(x * topRadius, y * topRadius, z));
    }

    // put indices for sides
    unsigned int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // bebinning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector
            indices.push_back(k1); indices.push_back(k1 + 1); indices.push_back(k2);
            indices.push_back(k2); indices.push_back(k1 + 1); indices.push_back(k2 + 1);
        }
    }

    // remember where the base indices start
    baseIndex = (unsigned int)indices.size();

    // put indices for base
    for (int i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < (sectorCount - 1))
        {
            indices.push_back(baseVertexIndex); indices.push_back(k + 1); indices.push_back(k);
        }
        else
        {
            indices.push_back(baseVertexIndex); indices.push_back(baseVertexIndex + 1); indices.push_back(k);
        }
    }

    // remember where the base indices start
    topIndex = (unsigned int)indices.size();

    for (int i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < (sectorCount - 1))
        {
            indices.push_back(topVertexIndex); indices.push_back(k); indices.push_back(k + 1);
        }
        else
        {
            indices.push_back(topVertexIndex); indices.push_back(k); indices.push_back(topVertexIndex + 1);
        }
    }

    // generate interleaved vertex array as well
    //buildInterleavedVertices();
}