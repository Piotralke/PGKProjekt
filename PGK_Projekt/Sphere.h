#pragma once
#ifndef sphere_h
#define sphere_h
#include "Cuboid.h"
#include <vector>
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Sphere
{
protected:
	std::vector<Vertex> vertices;

	unsigned int VBO, VAO, EBO;

	glm::vec3 position;

	glm::vec3 scalingFactor;

	float rotationAngleX=0;

	float rotationAngleY=0;

	float rotationAngleZ=0;
	std::vector<unsigned int> indices;
public:

	Sphere(glm::vec3 point, float radius, int sectorCount, int stackCount)
	{
		float x, y, z, xy; // Pozycja i wspó³rzêdne tekstur
		float sectorStep = 2 * glm::pi<float>() / sectorCount;
		float stackStep = glm::pi<float>() / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i) {
			stackAngle = glm::pi<float>() / 2 - i * stackStep;
			xy = radius * glm::cos(stackAngle);
			z = radius * glm::sin(stackAngle);

			for (int j = 0; j <= sectorCount; ++j) {
				sectorAngle = j * sectorStep;
				x = xy * glm::cos(sectorAngle);
				y = xy * glm::sin(sectorAngle);
				Vertex vertex;
				vertex.Position = glm::vec3(x, y, z);
				vertex.Normal = glm::normalize(vertex.Position);
				vertex.TexCoords = glm::vec2((float)j / sectorCount, (float)i / stackCount);
				vertices.push_back(vertex);
			}
		}

		for (int i = 0; i < stackCount; ++i) {
			for (int j = 0; j < sectorCount; ++j) {
				int k1 = i * (sectorCount + 1) + j;
				int k2 = k1 + 1;
				int k3 = (i + 1) * (sectorCount + 1) + j;
				int k4 = k3 + 1;

				indices.push_back(k1);
				indices.push_back(k3);
				indices.push_back(k4);
				indices.push_back(k1);
				indices.push_back(k4);
				indices.push_back(k2);
			}
		}
		this->scalingFactor = glm::vec3(1.0, 1.0, 1.0);
		position = point;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Atrybuty wierzcho³ków
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void draw(Shader& shader, unsigned int texture, int textureNum, Camera& camera);

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

#endif // !sphere_h