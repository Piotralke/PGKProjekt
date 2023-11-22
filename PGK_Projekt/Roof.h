#pragma once
#ifndef ROOF_H
#define ROOF_H

#include "Cube.h"

class Roof
{
protected:
    float vertices[128] = {
    // Base
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,

    // Side 1
    0.0f,  0.15f,  0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    // Side 2
    0.0f,  0.15f,  0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f,

    // Side 3
    0.0f,  0.15f,  0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    // Side 4
    0.0f,  0.15f,  0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f
};


    unsigned int VBO, VAO, EBO;

    glm::vec3 position;
    glm::vec3 scalingFactor;

    float rotationAngleX = 0;
    float rotationAngleY = 0;
    float rotationAngleZ = 0;

    unsigned int indices[18] = {
    // Base
    0, 1, 2, 2, 3, 0,

    // Side 1
    4, 5, 6,

    // Side 2
    7, 8, 9,

    // Side 3
    10, 11, 12,

    // Side 4
    13, 14, 15
};

public:
    Roof() {}
    Roof(glm::vec3 point)
    {
        scalingFactor = glm::vec3(1.0, 1.0, 1.0);
        position = point;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void draw(Shader& shader, unsigned int texture, int textureNum, Camera& camera, Shader& blurShader);

    void translate(glm::vec3 translation);

    void rotateX(float angle);

    void rotateY(float angle);

    void rotateZ(float angle);

    void scale(float k);

    glm::vec3 getPosition();

    glm::vec3 getScalingFactor();

    void setPosition(glm::vec3 position);

    float getRotX();

    float getRotY();

    float getRotZ();
};
#endif