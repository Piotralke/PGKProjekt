#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H
#include <glad/glad.h>
#include <vector>
#include <glm.hpp>
using namespace std;
class Circle {
private:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    unsigned int vertexBuffer;
    unsigned int vertexArray;
    unsigned int elementBuffer;
public:
    Circle(float radius, int vCount)
{
    float angle = 360.0f / vCount;

    int triangleCount = vCount - 2;

    std::vector<glm::vec3> temp;
    // positions
    for (int i = 0; i < vCount; i++)
    {
        float currentAngle = angle * i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * sin(glm::radians(currentAngle));
        float z = 0.0f;

        vertices.push_back(glm::vec3(x, y, z));
    }

    // push indexes of each triangle points
    for (int i = 0; i < triangleCount; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
    // buffer
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    // element buffer object
    glGenBuffers(1, &elementBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // copying
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW); // asd

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW); // asd

}
    void draw(bool shouldDraw) {

        glBindVertexArray(vertexArray);
        if(shouldDraw)
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
};

#endif // !CIRCLE_H
