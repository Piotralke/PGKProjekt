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
	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f,  -1.0f,  0.0f, 0.0f,
		 1.0f,  -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
    };
	unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	const char* vertexShader = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 crntPos;
out vec3 Normal;
out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;


void main()
{
	crntPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
	Normal = aNormal;
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	gl_Position = camMatrix * vec4(crntPos, 1.0);
})";
	const char* fragmentShader = R"(#version 330 core
out vec4 FragColor;
in vec3 crntPos;
in vec3 Normal;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;


vec4 direcLight()
{
	float ambient = 0.50f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	return (texture(diffuse0, texCoord) * (diffuse + ambient)) * lightColor;
}


void main()
{
	FragColor = direcLight();
})";

	float focusDistance = 50.0f;
	float focusRange = 10.0f;
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
const char* vertexBlurShader = R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

out vec2 TexCoords;
out vec3 crntPos;

void main()
{
    crntPos = vec3(model * translation * rotation * scale * vec4(aPos, 0.0, 1.0));
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}
  )";
const char* fragmentBlurShader = R"(#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 crntPos;

uniform vec3 camPos;
uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform float focusDistance;
uniform float focusRange;
uniform float blur;

void main()
{
    vec3 col = vec3(0.0);
    float totalWeight = 0.0;

    for(float i = -1.0; i <= 1.0; i += 1.0)
    {
        for(float j = -1.0; j <= 1.0; j += 1.0)
        {
            vec2 offset = vec2(i, j) * 1/(10*blur);
            vec3 color = texture(screenTexture, TexCoords + offset).rgb;

             vec2 depthTexCoord = (TexCoords + offset) * 0.5 + 0.5;  // Przelicz współrzędne tekstury dla bufora głębokości
            float depth = texture(depthTexture, depthTexCoord).r;
			float depthDifference = abs(depth - focusDistance);
			
            float bokehFactor = smoothstep(focusDistance - focusRange, focusDistance + focusRange, depth);
			// bokehFactor *= exp(-pow(depthDifference / focusRange, 2.0));
			bokehFactor *= smoothstep(1.0, 0.0, depthDifference );

            float weight = 1.0 - length(offset) / 1.4142;

            totalWeight += weight;
            col += color * weight * bokehFactor;
        }
    }

    FragColor = vec4(col / totalWeight, 1.0);
}

)";

	Shader circleShader(vertexShaderSource, fragmentShaderSource);
	Shader shaderProgram(vertexShader, fragmentShader);
	Shader skyboxShader(skyboxVertex, skyboxFragment);
	Shader blurShader(vertexBlurShader, fragmentBlurShader);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 100.0f, -200.0f);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	
	// unsigned int framebuffer;
	// glGenFramebuffers(1, &framebuffer);
	// glUniform1i(glGetUniformLocation(blurShader.ID, "depthTexture"), 0);
	// glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	/*unsigned int depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1080, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);*/
	GLuint framebuffer;
glGenFramebuffers(1, &framebuffer);
glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

GLuint depthTexture;
glGenTextures(1, &depthTexture);
glBindTexture(GL_TEXTURE_2D, depthTexture);
glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, getWidth(), getHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1080, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1080, 1080); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);

	Camera camera(getWidth(), getHeight(), glm::vec3(0.0f, 0.0f, 2.0f)); 
	setBackgroundColor(255.0f, 100.0f, 0.0f, 100.0f);
	glm::vec3 red(1.0f, 0.0f, 0.0f);
	glm::vec3 pos(5.0f, 0.0f, 0.0f);
	glm::vec3 pos2(0.0f, 0.0f, 5.0f);
	glm::vec3 posHouse1(-5.0f, 5.0f, 0.0f);
	glm::vec3 posHouse2(-50.0f, 5.0f, 4.0f);
	glm::vec3 pos69(-50.0f, 0.0f, 150.0f);
	glm::vec3 pos33(0.0f, 0.0f, 0.0f);
	glm::vec3 pos4(0.0f, 0.0f, 0.0f);
	Cube cube2(pos2);
	Cuboid floor(pos4,400.0f,1.0f,400.0f);
	Sphere sphere(pos, 1.0f, 20, 20);
	Skybox skybox;
	unsigned int cubemapTexture = skybox.getCubemapTexture();
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	unsigned int grassTexture = bitmapHandler->loadTexture((parentDir + "\\grass.jpg").c_str());
	unsigned int brickTexture = bitmapHandler->loadTexture((parentDir + "\\brick.jpg").c_str());
	unsigned int woodTexture = bitmapHandler->loadTexture((parentDir + "\\wood.jpg").c_str());
	unsigned int windowTexture = bitmapHandler->loadTexture((parentDir + "\\okno.jpg").c_str());
	unsigned int doorTexture = bitmapHandler->loadTexture((parentDir + "\\door.jpg").c_str());
	unsigned int roofTexture = bitmapHandler->loadTexture((parentDir + "\\roof.jpg").c_str());

	House1 brickHouse(posHouse1, woodTexture, 0, windowTexture, 0, doorTexture, 0, roofTexture, 0, 1);
	House1 brickHouse2(posHouse2, woodTexture, 0, windowTexture, 0, doorTexture, 0, roofTexture, 0, 1);
	vector<House1> houses;
	Circle circle(0.8,64);
	camera.updateMatrix(45.0f, 0.1f, 1000.0f);
	for (int i = -195; i < 200; i = i + 30)
	{
		for (int j = -195; j < 200; j = j + 30)
		{
			glm::vec3 positionHouse(i, 5, j);
			House1 newHouse(positionHouse, woodTexture, 0, windowTexture, 0, doorTexture, 0, roofTexture, 0, 1);
			houses.push_back(newHouse);
		}
		
	}
	blurShader.Activate();
	glUniform1i(glGetUniformLocation(blurShader.ID, "screenTexture"), 0);
	glUniform1i(glGetUniformLocation(blurShader.ID, "depthTexture"), 1);
	while (!glfwWindowShouldClose(getWindow()))
	{

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		processInput();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearStencil(0);
		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST); 
		glStencilFunc(GL_ALWAYS, 1, 0xFF); //
		glStencilMask(0xFF);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); 

		camera.Inputs(window);

		circleShader.Activate();
		circle.draw(camera.shouldDraw);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); 
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glStencilFunc(GL_EQUAL, 1, 0xFF); 

		glStencilMask(0x00);
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		
		shaderProgram.Activate();

		for (int i = 0; i < houses.size() - 1; i++)
		{
			houses.at(i).draw(shaderProgram, camera, blurShader);
		}

		/*brickHouse.draw(shaderProgram, camera);
		brickHouse2.draw(shaderProgram, camera);*/

		floor.draw(shaderProgram, grassTexture, 0, camera, blurShader);
		
		
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float) getWidth() / getHeight(), 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(skybox.getVAO());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearStencil(0);
		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF); 
		glStencilMask(0xFF);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		camera.Inputs(window);

		circleShader.Activate();
		circle.draw(camera.shouldDraw);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glStencilFunc(GL_EQUAL, 1, 0xFF);

		glStencilMask(0x00);
		glStencilFunc(GL_EQUAL, 1, 0xFF);

		shaderProgram.Activate();

		for (int i = 0; i < houses.size() - 1; i++)
		{
			houses.at(i).draw(shaderProgram, camera, blurShader);
		}

		brickHouse.draw(shaderProgram, camera, blurShader);
		brickHouse2.draw(shaderProgram, camera, blurShader);

		floor.draw(shaderProgram, grassTexture, 0, camera, blurShader);


		glDepthFunc(GL_LEQUAL);

		skyboxShader.Activate();
		view = glm::mat4(1.0f);
		proj = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)getWidth() / getHeight(), 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(skybox.getVAO());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);



		glDepthFunc(GL_LESS);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_DEPTH_TEST);
    blurShader.Activate();
	    glUniform1i(glGetUniformLocation(blurShader.ID, "screenTexture"), 0);
    glUniform1i(glGetUniformLocation(blurShader.ID, "depthTexture"), 1);
    glUniform1f(glGetUniformLocation(blurShader.ID, "focusDistance"), camera.focusDistance);
    glUniform1f(glGetUniformLocation(blurShader.ID, "blur"), camera.blur);
    glUniform1f(glGetUniformLocation(blurShader.ID, "focusRange"), camera.focalRange);

    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	blurShader.Delete();
	shaderProgram.Delete();
	skyboxShader.Delete();
	circleShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
}