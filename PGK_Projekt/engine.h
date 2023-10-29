#pragma once
#ifndef engine_h
#define engine_h
#include <string>
#include "Camera.h"
#include "Cube.h"
#include "BitmapHandler.h"
#include <time.h>
#include <filesystem>
#include <iostream>

void MouseCallback(GLFWwindow* window, double xpos, double ypos);

//void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

class Engine
{
private:
	Camera* camera;

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

	/*!<Vertex Shader*/
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"

		"out vec2 texCoords;\n"

		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
    	"{\n"
		"   vec4 pos = projection * view * vec4(aPos, 1.0f);\n"	
    	"   gl_Position = vec4(pos.x, pos.y, pos.z, pos.w);\n"
		"	texCoords = vec3(aPos.x, aPos.y,-aPos.z);\n"
    	"}\0";
	/*!<Fragment Shader*/
	const char *fragmentShaderSource ="#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoord;\n"
		"uniform samplerCube skybox;\n"
		"void main()\n"
    	"{\n"
    	"  FragColor = texture(skybox, TexCoord);\n"
		"}\0";
	/*!<Vertex Lightning Shader*/
	const char* vertexLightningShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoords;\n"
		"out vec3 FragPos;\n"
		"out vec2 TexCoords;\n"
		"out vec3 Normal;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		"void main()\n"
		"{\n"
		"FragPos = vec3(model * vec4(aPos, 1.0));\n"
		"Normal = mat3(transpose(inverse(model))) * aNormal;\n"
		"TexCoords = aTexCoords;\n"
		"gl_Position = projection * view * vec4(FragPos, 1.0);\n"
		"}\0";
		/*!<Fragment lightning shader*/
	const char* fragmentLightningShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"struct Material {\n"
		"sampler2D diffuse;\n"
		"vec3 specular;\n"
		"float shininess;\n"
		"};\n"
		"struct Light {\n"
		"vec3 position;\n"
		"vec3 ambient;\n"
		"vec3 diffuse;\n"
		"vec3 specular;\n"
		"};\n"
		"in vec3 FragPos;\n"
		"in vec3 Normal;\n"
		"in vec2 TexCoords;\n"

		"uniform vec3 viewPos;\n"
		"uniform Material material;\n"
		"uniform Light light;\n"

		"void main()\n"
		"{\n"
		"vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;\n"
		"vec3 norm = normalize(Normal);\n"
		"vec3 lightDir = normalize(light.position - FragPos);\n"
		"float diff = max(dot(norm, lightDir), 0.0);\n"
		"vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;\n"
		"vec3 viewDir = normalize(viewPos - FragPos);\n"
		"vec3 reflectDir = reflect(-lightDir, norm);\n"
		"float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); \n"
		"vec3 specular = light.specular * (spec * material.specular); \n"
		"vec3 result = ambient + diffuse + specular;\n"
		"FragColor = vec4(result, 1.0);\n"
		"}\0";
	
		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int programShader2;
		unsigned int programShader;
		unsigned int VBO, VAO;
		unsigned int vertexLightningShader;
		unsigned int fragmentLightningShader;
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
		
		int success;
		char infoLog[512];
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glEnable(GL_DEPTH_TEST);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		programShader = glCreateProgram();
		glAttachShader(programShader, vertexShader);
		glAttachShader(programShader, fragmentShader);
		glLinkProgram(programShader);

		vertexLightningShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexLightningShader, 1, &vertexLightningShaderSource, NULL);
		glCompileShader(vertexLightningShader);
		glGetShaderiv(vertexLightningShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexLightningShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		fragmentLightningShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentLightningShader, 1, &fragmentLightningShaderSource, NULL);
		glCompileShader(fragmentLightningShader);
		glGetShaderiv(fragmentLightningShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentLightningShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		programShader2 = glCreateProgram();
		glAttachShader(programShader2, vertexLightningShader);
		glAttachShader(programShader2, fragmentLightningShader);
		glLinkProgram(programShader2);
		glGetProgramiv(programShader2, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programShader2, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexLightningShader);
		glDeleteShader(fragmentLightningShader);

		

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
     	this->screenHeight = height;
		this->screenWidth = width;
		this->monitor = monitor;
		camera = new Camera(width, height,programShader2,programShader2, window);
		//glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetCursorPosCallback(window, MouseCallback);
		
	}
	
	void setWindowSize(unsigned int width, unsigned int height);

	void isFullscreen(bool fullscreen);

	void init();

	void processInput();

	void mainLoop();

	void setBackgroundColor(float r,float g,float b,float a);

	static Engine* getInstance()
	{
		return instance;
	}
	Camera* getCamera()
	{
		return camera;
	}
	GLFWwindow* getWindow();
};
#endif // !engine_h
