#include "House1.h"
#include <gtc/type_ptr.hpp>

void House1::draw(Shader& shader, Camera& camera)
{
	this->cube->draw(shader, this->textureCube, this->textureCubeNumber, camera);
	this->window->draw(shader, this->textureWindow, this->textureWindowNumber, camera);
	this->door->draw(shader, this->textureDoor, this->textureDoorNumber, camera);
    this->roof->draw(shader, this->textureRoof, this->textureRoofNumber, camera);
}

void House1::translate(glm::vec3 translation)
{
	position += translation;
	this->cube->translate(translation);
	this->window->translate(translation);
	this->door->translate(translation);
	this->roof->translate(translation);
}
void House1::rotateY(float angle)
{
	rotationAngleY += angle;
	this->cube->rotateY(angle);
	this->window->rotateY(angle);
	this->door->rotateY(angle);
	this->roof->rotateY(angle);
}

void House1::scale(float k)
{
	glm::vec3 factor(k, k, k);
	scalingFactor *= factor;
	this->cube->scale(k);
	this->window->scale(k);
	glm::vec3 roofTrans(0, k*10, 0);
	glm::vec3 windowTrans(5*k, 0, 0);
	glm::vec3 doorTrans(0, -2*k, 5*k);
	this->window->translate(windowTrans);
	this->door->translate(doorTrans);
	this->roof->scale(k);
	this->roof->translate(roofTrans);
	this->door->scale(k);
}
glm::vec3 House1::getPosition()
{
	return position;
}
glm::vec3 House1::getScalingFactor()
{
	return scalingFactor;
}
float House1::getRotX()
{
	return rotationAngleX;
}
float House1::getRotY()
{
	return rotationAngleY;
}
float House1::getRotZ()
{
	return rotationAngleZ;
}