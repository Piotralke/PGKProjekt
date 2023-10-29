#include "Camera.h"
void Camera::UpdateCamera(unsigned int programShader, unsigned int programShader2)
{
    glUseProgram(programShader);
    view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(glGetUniformLocation(programShader, "view"), 1, GL_FALSE, &view[0][0]);

}
void Camera::VerticalMove(bool isUp, float cameraSpeed)
{
    if(isUp)
        cameraPos += cameraSpeed * glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
    else
        cameraPos -= cameraSpeed * glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
}
void Camera::HorizontalMove(bool isRight,float cameraSpeed)
{
    if(isRight)
        cameraPos += glm::normalize(glm::cross(glm::vec3(cameraFront.x, 0.0f, cameraFront.z), cameraUp)) * cameraSpeed;
    else
        cameraPos -= glm::normalize(glm::cross(glm::vec3(cameraFront.x, 0.0f, cameraFront.z),cameraUp)) * cameraSpeed;
}
void Camera::UpdateMouse(double xPos, double yPos)
{
	float xpos = static_cast<float>(xPos);
	float ypos = static_cast<float>(yPos);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; 
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

glm::mat4 Camera::getView()
{
	return view;
}

glm::vec3 Camera::getCameraFront()
{
	return cameraFront;
}
glm::vec3 Camera::getCameraPos()
{
	return cameraPos;
}
void Camera::setCameraPos(glm::vec3 pos)
{
	cameraPos = pos;
}