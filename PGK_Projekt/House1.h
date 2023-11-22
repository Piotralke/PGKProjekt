#pragma once
#ifndef HOUSE1_H
#define HOUSE1_H
#include "Sphere.h"
#include "Roof.h"
class House1 {
private:
	Cube* cube;
	Cuboid* window;
	Cuboid* door;
	Roof* roof;
	unsigned int textureCube, textureWindow, textureDoor, textureRoof;
	int textureCubeNumber, textureWindowNumber, textureDoorNumber, textureRoofNumber;

	unsigned int VBO, VAO, EBO;

	glm::vec3 position;

	glm::vec3 scalingFactor;

	float rotationAngleX = 0;

	float rotationAngleY = 0;

	float rotationAngleZ = 0;

	void scale(float k);
	void rotateY(float angle);

public:
	House1(glm::vec3 position, unsigned int textureCube,int textureCubeNumber, 
		unsigned int textureWindow, int textureWindowNumber, 
		unsigned int textureDoor, int textureDoorNumber,
		unsigned int textureRoof, int textureRoofNumber,
		float scale
	)
	{
		this->textureCube = textureCube;
		this->textureDoor = textureDoor;
		this->textureWindow = textureWindow;
		this->textureRoof = textureRoof;
		this->textureCubeNumber = textureCubeNumber;
		this->textureDoorNumber = textureDoorNumber;
		this->textureWindowNumber = textureWindowNumber;
		this->textureRoofNumber = textureRoofNumber;
		this->roof = new Roof(position);
		this->cube = new Cube(position);
		this->cube->scale(10);
		this->roof->scale(11);
		this->window = new Cuboid(position, 0.1f, 3.0f, 4.0f);
        this->door = new Cuboid(position, 4.0f, 6.0f, 0.1f);
		this->scale(scale);
		
	}
	void draw(Shader& shader, Camera& camera,Shader& blurShader);
	void translate(glm::vec3 translation);	

	glm::vec3 getPosition();

	glm::vec3 getScalingFactor();

	void setPosition(glm::vec3 position);

	float getRotX();

	float getRotY();

	float getRotZ();
};


#endif