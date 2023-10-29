#pragma once
#ifndef BitmapHandler_h
#define BitmapHandler_h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


class BitmapHandler {
public:

	BitmapHandler(){}

	unsigned int loadTexture(char const* path);
};

#endif BitmapHandler_h