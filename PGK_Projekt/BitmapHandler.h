#pragma once
#ifndef BitmapHandler_h
#define BitmapHandler_h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>


class BitmapHandler {
public:

	BitmapHandler(){}

	unsigned int loadTexture(char const* path);

	void loadSkybox(std::string facesCubemap[]);
};

#endif BitmapHandler_h