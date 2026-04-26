#pragma once

/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/

#include <GL/glew.h> // OpenGL Extension Wrangler Library: OpenGL header files <- GPU.
#include <string>

#define STB_IMAGE_IMPLEMENTATION // Navigate to stb_image.h to include in the project.
#include "Dependencies/stb-master/stb_image.h"


class TEXTURE {
private:

public:
	TEXTURE() {

	}
	~TEXTURE() {

	}
	GLuint GetTextureID(std::string texturePath) {
		int width, height, channels; // channels: 3 -> rgb -> JPEG. 4 -> rgb-alpha -> PNG.

		stbi_uc* image = stbi_load(
			texturePath.c_str(),
			&width,
			&height,
			&channels,
			STBI_rgb // rgb channels.
		);
	}
};