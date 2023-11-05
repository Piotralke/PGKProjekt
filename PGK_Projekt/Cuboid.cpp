#include "Cuboid.h"
#include <gtc/type_ptr.hpp>

void Cuboid::draw(Shader& shader, unsigned int texture, int textureNum, Camera& camera)
{
	glBindVertexArray(this->VAO);
	glActiveTexture(GL_TEXTURE0 + textureNum);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader.ID, "diffuse0"), textureNum);
	glUniform1i(glGetUniformLocation(shader.ID, "specular0"), textureNum);
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera.cameraMatrix));
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(2.0f);
	glm::mat4 matrix = glm::mat4(1.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	// Transform the matrices to their correct form
	trans = glm::translate(trans, this->position);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scalingFactor);

	// Push the matrices to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


void Cuboid::translate(glm::vec3 translation)
{
	position += translation;
}
void Cuboid::setPosition(glm::vec3 position)
{
	this->position = position;
}
void Cuboid::rotateX(float angle)
{
	rotationAngleX += angle;
}
void Cuboid::rotateY(float angle)
{
	rotationAngleY += angle;
}
void Cuboid::rotateZ(float angle)
{
	rotationAngleZ += angle;
}
void Cuboid::scaleX(float k)
{
	glm::vec3 factor(k, scalingFactor.y, scalingFactor.z);
	scalingFactor *= factor;
}
void Cuboid::scaleY(float k)
{
	glm::vec3 factor(scalingFactor.x, k, scalingFactor.z);
	scalingFactor *= factor;
}
void Cuboid::scaleZ(float k)
{
	glm::vec3 factor(scalingFactor.x, scalingFactor.y, k);
	scalingFactor *= factor;
}
glm::vec3 Cuboid::getPosition()
{
	return position;
}
glm::vec3 Cuboid::getScalingFactor()
{
	return scalingFactor;
}
float Cuboid::getRotX()
{
	return rotationAngleX;
}
float Cuboid::getRotY()
{
	return rotationAngleY;
}
float Cuboid::getRotZ()
{
	return rotationAngleZ;
}