#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "BitmapHandler.h"
#include <iostream>
class Skybox {

private:
	float skyboxVertices[24] =
	{
		//   Coordinates
		-1.0f, -1.0f,  1.0f,//        7--------6
		 1.0f, -1.0f,  1.0f,//       /|       /|
		 1.0f, -1.0f, -1.0f,//      4--------5 |
		-1.0f, -1.0f, -1.0f,//      | |      | |
		-1.0f,  1.0f,  1.0f,//      | 3------|-2
		 1.0f,  1.0f,  1.0f,//      |/       |/
		 1.0f,  1.0f, -1.0f,//      0--------1
		-1.0f,  1.0f, -1.0f
	};

	unsigned int skyboxIndices[36] =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int cubemapTexture;
	std::string facesCubemap[6] =
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
public:
	Skybox() {
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glGenBuffers(1, &skyboxEBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenTextures(1, &cubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// These are very important to prevent seams
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// This might help with seams on some systems
		//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		// Cycles through all the textures and attaches them to the cubemap object
		BitmapHandler bh;
		
		bh.loadSkybox(facesCubemap);


	}
	unsigned int getCubemapTexture();
	unsigned int getVAO();
};

#endif // !SKYBOX_H
