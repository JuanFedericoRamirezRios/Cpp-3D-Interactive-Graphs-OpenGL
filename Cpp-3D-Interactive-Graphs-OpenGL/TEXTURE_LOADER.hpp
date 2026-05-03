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


class TEXTURE_LOADER {
private:

public:
	TEXTURE_LOADER() {

	}
	~TEXTURE_LOADER() {

	}
	GLuint GetTextureID(std::string texturePath) {
		// ******* Load the image data *******
		int width, height, channels; // channels of JPEG: 3 -> rgb. channels fo a PNG: 4 -> rgb-alpha.
		stbi_uc* image = stbi_load( // uc: unsigned char pointer
			texturePath.c_str(),
			&width,
			&height,
			&channels,
			STBI_rgb // rgb channels -> JPEG images.
		);

		// ******* Bind the texture *******
		GLuint texture;
		glGenTextures(1, &texture); // 1 object
		glBindTexture(GL_TEXTURE_2D, texture);

		// ******* Texture wrapping *******
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // s coordinate -> x
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // t coordinate -> y

		// ******* Set the texture filtering *******
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // When texture is minified (far camera).
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // When texture is minified (far camera).
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // When texture is magnified (near camera).

		// ******* Create the texture *******
		glTexImage2D(
			GL_TEXTURE_2D, 
			0, // mipmap quality picture. 3, 2 or 1: low quality. 0: base level
			GL_RGB, // 3 channels
			width, 
			height, 
			0, // Always is 0.
			GL_RGB, // Data format of the image source.
			GL_UNSIGNED_BYTE, // type of data
			image
		);

		glGenerateMipmap(GL_TEXTURE_2D); // Generate Mipmap
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
		stbi_image_free(image); // free the picture
		
		return texture; // return texture
	}
};