#include "engine.h"
namespace fs = std::filesystem;
Engine* Engine::instance = NULL;
void Engine::init()
{
	glfwSetWindowSizeLimits(getWindow(), 1024, 576, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwMakeContextCurrent(this->getWindow());
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
	}
}
void Engine::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		isFullscreen(true);
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		isFullscreen(false);
	float cameraSpeed = static_cast<float>(6.0 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->VerticalMove(true, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->VerticalMove(false, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->HorizontalMove(false, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->HorizontalMove(true, cameraSpeed);
}
void Engine::setWindowSize(unsigned int width, unsigned int height)
{
	glfwSetWindowSize(this->window, width, height);
}
void Engine::isFullscreen(bool fullscreen)
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (fullscreen)
	{
		glfwSetWindowMonitor(this->window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(this->window, NULL, mode->width * 0.1, mode->height * 0.1, mode->width * 0.8, mode->height * 0.8, mode->refreshRate);
	}
}
GLFWwindow* Engine::getWindow()
{
	return this->window;
}
void Engine::setBackgroundColor(float r, float g, float b, float a)
{
	backgroundColor.r = r / 255;
	backgroundColor.g = g / 255;
	backgroundColor.b = b / 255;
	backgroundColor.a = a / 255;
}
void Engine::mainLoop()
{
	//Point3D point3D(-0.5f, -0.5f,0.0f);
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		-0.5f,  0.5f, 0.0f,  // top left    
	};
	float vertices2[] = {
		2.0f, 3.5f, -3.0f, // left     
	};
	float vertices3[] = {
		0.1f,  0.1f, 0.0f,  // top right
		 0.1f, -0.1f, 0.0f,  // bottom right
		-0.1f, -0.1f, 0.0f,  // bottom left
		-0.1f,  0.1f, 0.0f   // top left     
	};
	float vertices4[] = {
		0.7f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right    
	};
	float vertices5[] = {
		0.7f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.8f, 0.0f,   // top left   
	};
	float vertices6[] = {
		0.5f, 0.5f, 0.5f,       1.0f,0.0f,0.0f,//1 
		-0.5f, 0.5f, 0.5f,	    1.0f,0.0f,0.0f,//2
		-0.5f, 0.5f, -0.5f,	    1.0f,0.0f,0.0f,//3
		0.5f,0.5f,-0.5f,	    1.0f,0.0f,0.0f,//4
		0.5f, -0.5f, 0.5f,	    1.0f,0.0f,0.0f,//5
		-0.5f, -0.5f, 0.5f,	    1.0f,0.0f,0.0f,//6
		-0.5f, -0.5f, -0.5f,	1.0f,0.0f,0.0f,//7
		0.5f,-0.5f,-0.5f,	    1.0f,0.0f,0.0f//8


	};
	setBackgroundColor(255.0f, 100.0f, 0.0f, 100.0f);
	glm::vec3 red(1.0f, 0.0f, 0.0f);
	glm::vec3 pos(0.0f, -1.0f, 0.0f);
	Cube cube(pos, red);
	Cube skybox(pos, red);
	skybox.scale(100);

	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string texPath = "\\diffuse.jpg";
	unsigned int diffuseMap = bitmapHandler->loadTexture((parentDir + texPath).c_str());

	std::string texPath2 = "\\skybox.jpg";
	unsigned int diffuseMap2 = bitmapHandler->loadTexture((parentDir + texPath2).c_str());

	glEnable(GL_TEXTURE_2D);
	glUseProgram(programShader2);
	glUniform1i(glGetUniformLocation(programShader2, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(programShader, "skybox"), 0);
	while (!glfwWindowShouldClose(getWindow()))
	{

		glUseProgram(programShader2);
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput();
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programShader2);
		glUniform3f(glGetUniformLocation(programShader2, "light.position"), 0, 50, 0);
		glUniform3f(glGetUniformLocation(programShader2, "viewPos"), camera->getCameraPos().x, camera->getCameraPos().y, camera->getCameraPos().z);
		glUniform3f(glGetUniformLocation(programShader2, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(programShader2, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(programShader2, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(programShader2, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(programShader2, "material.shininess"), 64.0f);

		glUseProgram(programShader2);
		cube.draw(programShader2, diffuseMap, 1);
		
		glActiveTexture(GL_TEXTURE0);
		

		camera->UpdateCamera(programShader2, programShader2);

		glDepthFunc(GL_LEQUAL);
		skybox.draw(programShader, diffuseMap2, 2);
		glDepthFunc(GL_LESS);
		glfwSwapBuffers(getWindow());
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(programShader2);
	glfwTerminate();
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Engine* eng = Engine::getInstance();
	eng->getCamera()->UpdateMouse(xpos, ypos);
}

//void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
//{
//	Engine* eng = Engine::getInstance();
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		glm::vec3 view = eng->getCamera()->getCameraPos();
//		glm::vec3 direction = glm::normalize(eng->getCamera()->getCameraFront());
//
//	}
//
//	//
//}

//bool checkCollision(Cube ob1, Cube ob2)
//{
//	// collision x-axis
//	bool collisionX = ob1.getPosition().x + 0.5 * ob1.getScalingFactor().x >= ob2.getPosition().x &&
//		ob2.getPosition().x + 0.5 * ob1.getScalingFactor().x >= ob1.getPosition().x;
//	// collision y-axis
//	bool collisionY = ob1.getPosition().y + 0.5 * ob1.getScalingFactor().y >= ob2.getPosition().y &&
//		ob2.getPosition().y + 0.5 * ob1.getScalingFactor().y >= ob1.getPosition().y;
//	// collision z-axis
//	bool collisionZ = ob1.getPosition().z + 0.5 * ob1.getScalingFactor().z >= ob2.getPosition().z &&
//		ob2.getPosition().z + 0.5 * ob1.getScalingFactor().z >= ob1.getPosition().z;
//	return collisionX && collisionY && collisionZ;
//}