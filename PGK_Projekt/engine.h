#pragma once
#ifndef engine_h
#define engine_h
#include "House1.h"
#include "Skybox.h"
#include <time.h>
#include <filesystem>
#include <string>
class Engine
{
private:

	BitmapHandler* bitmapHandler;

	static Engine* instance;

	GLFWwindow* window;

	GLFWmonitor* monitor;

	unsigned int screenWidth;

	unsigned int screenHeight;

	float deltaTime;

	float lastFrame;

	glm::mat4 projection;

	glm::vec4 backgroundColor = glm::vec4(0.0f,0.0f,1.0f,0.1f);

	unsigned int VBO, VAO;

public:

	Engine(unsigned int width, unsigned int height, std::string Title, GLFWmonitor* monitor)
	{
		
		this->instance = this;
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glEnable(GL_DEPTH_TEST);
		this->window = glfwCreateWindow(width, height, Title.c_str(), monitor, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		
		init();
		

		
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
     	this->screenHeight = height;
		this->screenWidth = width;
		this->monitor = monitor; 

		//glfwSetMouseButtonCallback(window, MouseButtonCallback);
		/*glfwSetCursorPosCallback(window, MouseCallback);*/
		
	}
	
	void setWindowSize(unsigned int width, unsigned int height);

	void isFullscreen(bool fullscreen);

	void init();

	void processInput();

	void mainLoop();

	unsigned int getWidth();
	unsigned int getHeight();

	void setBackgroundColor(float r,float g,float b,float a);

	static Engine* getInstance()
	{
		return instance;
	}
	GLFWwindow* getWindow();
};
#endif // !engine_h
