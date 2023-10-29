#include "Cube.h"
void Cube::draw(unsigned int programShader,unsigned int texture,int textureNum)
{
	glUseProgram(programShader);
	glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
	model = glm::rotate(model, rotationAngleX,glm::vec3(1.0,0.0,0.0));
	model = glm::rotate(model, rotationAngleY, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, rotationAngleZ, glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, scalingFactor);
    glUniformMatrix4fv(glGetUniformLocation(programShader, "model"), 1, GL_FALSE, &model[0][0]);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void Cube::setVertices(float vertices[])
{
	for (int i = 0; i < 24; i++)
	{
		this->vertices[i] = vertices[i];
	}
}
float* Cube::getVertices()
{
	return this->vertices;
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