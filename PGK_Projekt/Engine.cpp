#include "engine.h"
namespace fs = std::filesystem;
Engine* Engine::instance = NULL;
void Engine::init()
{
	glfwSetWindowSizeLimits(getWindow(), 1080, 1080, GLFW_DONT_CARE, GLFW_DONT_CARE);
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
unsigned int Engine::getWidth()
{
	return this->screenWidth;
}
unsigned int Engine::getHeight()
{
	return this->screenHeight;
}

void Engine::mainLoop()
{
	//Point3D point3D(-0.5f, -0.5f,0.0f);
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


	const char* vertexShader = R"(#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
//layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;


// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
//out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;



// Imports the camera matrix
uniform mat4 camMatrix;
// Imports the transformation matrices
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;


void main()
{
	// calculates current position
	crntPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
	// Assigns the normal from the Vertex Data to "Normal"
	Normal = aNormal;
	// Assigns the colors from the Vertex Data to "color"
	/*color = aColor;*/
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.0);
})";
	const char* fragmentShader = R"(#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
//in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;


// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
//uniform sampler2D specular0;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;


//vec4 pointLight()
//{	
//	// used in two variables so I calculate it here to not have to do it twice
//	vec3 lightVec = lightPos - crntPos;
//
//	// intensity of light with respect to distance
//	float dist = length(lightVec);
//	float a = 3.0;
//	float b = 0.7;
//	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);
//
//	// ambient lighting
//	float ambient = 0.20f;
//
//	// diffuse lighting
//	vec3 normal = normalize(Normal);
//	vec3 lightDirection = normalize(lightVec);
//	float diffuse = max(dot(normal, lightDirection), 0.0f);
//
//	// specular lighting
//	float specularLight = 0.50f;
//	vec3 viewDirection = normalize(camPos - crntPos);
//	vec3 reflectionDirection = reflect(-lightDirection, normal);
//	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
//	float specular = specAmount * specularLight;
//
//	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
//}

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.50f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	//float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	//float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse + ambient)) * lightColor;
}

//vec4 spotLight()
//{
//	// controls how big the area that is lit up is
//	float outerCone = 0.90f;
//	float innerCone = 0.95f;
//
//	// ambient lighting
//	float ambient = 0.20f;
//
//	// diffuse lighting
//	vec3 normal = normalize(Normal);
//	vec3 lightDirection = normalize(lightPos - crntPos);
//	float diffuse = max(dot(normal, lightDirection), 0.0f);
//
//	// specular lighting
//	float specularLight = 0.50f;
//	vec3 viewDirection = normalize(camPos - crntPos);
//	vec3 reflectionDirection = reflect(-lightDirection, normal);
//	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
//	float specular = specAmount * specularLight;
//
//	// calculates the intensity of the crntPos based on its angle to the center of the light cone
//	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
//	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
//
//	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
//}


void main()
{
	// outputs final color
	FragColor = direcLight();
})";

const char* skyboxVertex = R"(#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec4 pos = projection * view * vec4(aPos, 1.0f);
    // Having z equal w will always result in a depth of 1.0f
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    // We want to flip the z axis due to the different coordinate systems (left hand vs right hand)
    texCoords = vec3(aPos.x, aPos.y, -aPos.z);
}   )";
const char* skyboxFragment = R"(#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, texCoords);
})";

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 position;\n"
"void main() { gl_Position = vec4(position, 0.0, 1.0); }";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() { FragColor = vec4(0.0, 0.0, 0.0, 0.0); }";

	Shader circleShader(vertexShaderSource, fragmentShaderSource);
	Shader shaderProgram(vertexShader, fragmentShader);
	Shader skyboxShader(skyboxVertex, skyboxFragment);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 100.0f, -200.0f);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_STENCIL_TEST);

	//// Enables Cull Facing
	//glEnable(GL_CULL_FACE);
	//// Keeps front faces
	//glCullFace(GL_FRONT);
	//// Uses counter clock-wise standard
	//glFrontFace(GL_CCW);

	



	Camera camera(getWidth(), getHeight(), glm::vec3(0.0f, 0.0f, 2.0f)); 
	// GLUquadric* quad;
	// quad = gluNewQuadric();
	// gluSphere(quad, 25, 100, 20);
	setBackgroundColor(255.0f, 100.0f, 0.0f, 100.0f);
	glm::vec3 red(1.0f, 0.0f, 0.0f);
	glm::vec3 pos(5.0f, 0.0f, 0.0f);
	glm::vec3 pos2(0.0f, 0.0f, 5.0f);
	glm::vec3 posHouse1(-5.0f, 5.0f, 0.0f);
	glm::vec3 posHouse2(-13.0f, 5.0f, 4.0f);
	glm::vec3 pos69(-50.0f, 0.0f, 150.0f);
	glm::vec3 pos33(0.0f, 0.0f, 0.0f);
	glm::vec3 pos4(0.0f, 0.0f, 0.0f);
	/*Cube cube(pos);*/
	Cube cube2(pos2);
	/*Cuboid windowModel(pos33, 0.1f, 1.5f, 2.0f);*/
	Cuboid floor(pos4,400.0f,1.0f,400.0f);
	Sphere sphere(pos, 1.0f, 20, 20);
	Skybox skybox;
	unsigned int cubemapTexture = skybox.getCubemapTexture();
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
//	unsigned int diffuseMap = bitmapHandler->loadTexture((parentDir + "\\diffuse.jpg").c_str());
	unsigned int grassTexture = bitmapHandler->loadTexture((parentDir + "\\grass.jpg").c_str());
	//unsigned int houseTexture1 = bitmapHandler->loadTexture((parentDir + "\\house1.jpg").c_str());
	//unsigned int houseTexture2 = bitmapHandler->loadTexture((parentDir + "\\house2.jpg").c_str());
	//unsigned int houseTexture3 = bitmapHandler->loadTexture((parentDir + "\\house3.jpg").c_str());
	//unsigned int houseTexture4 = bitmapHandler->loadTexture((parentDir + "\\house4.jpg").c_str());
	//unsigned int houseTexture5 = bitmapHandler->loadTexture((parentDir + "\\house5.jpg").c_str());
	//unsigned int houseTexture6 = bitmapHandler->loadTexture((parentDir + "\\house6.jpg").c_str());
	//unsigned int houseTexture7 = bitmapHandler->loadTexture((parentDir + "\\house7.jpg").c_str());
	//unsigned int blokTexture1 = bitmapHandler->loadTexture((parentDir + "\\blok1.jpg").c_str());
	//unsigned int blokTexture2 = bitmapHandler->loadTexture((parentDir + "\\blok2.jpg").c_str());
	//unsigned int blokTexture3 = bitmapHandler->loadTexture((parentDir + "\\blok3.jpg").c_str());
	//unsigned int blokTexture4 = bitmapHandler->loadTexture((parentDir + "\\blok4.jpg").c_str());
	unsigned int brickTexture = bitmapHandler->loadTexture((parentDir + "\\brick.jpg").c_str());
	unsigned int woodTexture = bitmapHandler->loadTexture((parentDir + "\\wood.jpg").c_str());
	unsigned int windowTexture = bitmapHandler->loadTexture((parentDir + "\\okno.jpg").c_str());
	unsigned int doorTexture = bitmapHandler->loadTexture((parentDir + "\\door.jpg").c_str());
	unsigned int roofTexture = bitmapHandler->loadTexture((parentDir + "\\roof.jpg").c_str());

	House1 brickHouse(posHouse1, woodTexture, 0, windowTexture, 0, doorTexture, 0, roofTexture, 0, 1);
	House1 brickHouse2(posHouse2, woodTexture, 0, windowTexture, 0, doorTexture, 0, roofTexture, 0, 1);

	Circle circle(0.8,64);
	camera.updateMatrix(45.0f, 0.1f, 500.0f);
		/*std::vector<Vertex> sphereVertices;
	std::vector<unsigned int> sphereIndices;
	createSphere(sphereVertices, sphereIndices, 1.0f, 50, 50);
	std::cout << "VERTICES" << std::endl;
	for (int i = 0; i < sphereVertices.size(); i++)
	{
		std::cout << sphereVertices[i].Position.x << ", " << sphereVertices[i].Position.y
			<< ", " << sphereVertices[i].Position.z << ", " << sphereVertices[i].Normal.x
			<< ", " << sphereVertices[i].Normal.y
			<< ", " << sphereVertices[i].Normal.z << ", " << sphereVertices[i].TexCoords.x << ", " << sphereVertices[i].TexCoords.y << std::endl;
	}
	std::cout << "INDICES" << std::endl;
	for (int j = 0; j < sphereIndices.size(); j++)
	{
		std::cout << sphereIndices[j] << ", ";
	}*/
	//glEnable(GL_TEXTURE_2D);
	//glUseProgram(programShader2);
	//glUniform1i(glGetUniformLocation(programShader2, "material.diffuse"), 0);
	//glUniform1i(glGetUniformLocation(programShader, "skybox"), 0);
	while (!glfwWindowShouldClose(getWindow()))
	{

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClearStencil(0);
		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST); // Enables testing AND writing functionalities
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Do not test the current value in the stencil buffer, always accept any value on there for drawing
		glStencilMask(0xFF);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); // Make every test succeed

		camera.Inputs(window);

		
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);
		circleShader.Activate();
		circle.draw(camera.shouldDraw);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Make sure you will no longer (over)write stencil values, even if any test succeeds
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Make sure we draw on the backbuffer again.

		glStencilFunc(GL_EQUAL, 1, 0xFF); // Now we will only draw pixels where the corresponding stencil buffer value equals 1

		// glDisable(GL_BLEND);
		glStencilMask(0x00);
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		shaderProgram.Activate();
		/*cube.draw(shaderProgram, diffuseMap, 0, camera);*/
		/*sphere.draw(shaderProgram, diffuseMap, 0, camera);*/


		brickHouse.draw(shaderProgram, camera);
		brickHouse2.draw(shaderProgram, camera);

		floor.draw(shaderProgram, grassTexture, 0, camera);
		
		
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// The last row and column affect the translation of the skybox (which we don't want to affect)
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float) getWidth() / getHeight(), 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));



		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skybox.getVAO());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);
		glDisable(GL_STENCIL_TEST);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shaderProgram.Delete();
	skyboxShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	glfwTerminate();
}
//void MouseCallback(GLFWwindow* window, double xpos, double ypos)
//{
//	Engine* eng = Engine::getInstance();
//	eng->getCamera()->UpdateMouse(xpos, ypos);
//}

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