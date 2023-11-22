#include "Cube.h"
#include <gtc/type_ptr.hpp>

void Cube::draw(Shader& shader,unsigned int texture,int textureNum, Camera& camera, Shader& blurShader)
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
	rot = glm::rotate(rot, glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
	rot = glm::rotate(rot, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
	rot = glm::rotate(rot, glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
	// Transform the matrices to their correct form
	trans = glm::translate(trans, this->position);
	sca = glm::scale(sca, scalingFactor);
	
	// Push the matrices to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));
	
	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	blurShader.Activate();
	glUniform3f(glGetUniformLocation(blurShader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniformMatrix4fv(glGetUniformLocation(blurShader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(blurShader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(blurShader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(blurShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));
	shader.Activate();
}


void Cube::translate(glm::vec3 translation)
{
	position += translation;
}
void Cube::setPosition(glm::vec3 position)
{
	this->position = position;
}
void Cube::rotateX(float angle)
{
	rotationAngleX += angle;
}
void Cube::rotateY(float angle)
{
	rotationAngleY += angle;
}
void Cube::rotateZ(float angle)
{
	rotationAngleZ += angle;
}
void Cube::scale(float k)
{
	glm::vec3 factor(k, k, k);
	scalingFactor *= factor;
}
glm::vec3 Cube::getPosition()
{
	return position;
}
glm::vec3 Cube::getScalingFactor()
{
	return scalingFactor;
}
float Cube::getRotX()
{
	return rotationAngleX;
}
float Cube::getRotY()
{
	return rotationAngleY;
}
float Cube::getRotZ()
{
	return rotationAngleZ;
}