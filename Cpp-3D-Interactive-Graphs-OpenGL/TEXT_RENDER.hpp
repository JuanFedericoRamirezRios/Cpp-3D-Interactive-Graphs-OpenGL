#pragma once

/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/
#include <GL/glew.h> // OpenGL Extension Wrangler Library: OpenGL header files <- GPU.
#include "Dependencies/glm/glm/glm.hpp" // OpenGL maths.
#include "Dependencies/glm/glm/gtc/type_ptr.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"

#include <iostream> // This include std::cout and std::endl
#include <ft2build.h>
#include FT_FREETYPE_H // FT_FREETYPE_H macro just includes freetype.h
#include <string>
#include <map>

using namespace glm;

struct CHARACTER {
	GLuint textureID;	// Texture ID of each glyph texture
	ivec2 size;	// <- size of the glyph (vector information of TrueTypeFonts (TTF))
	ivec2 bearing;	// baseline to left/top of glyph
	GLuint advance;	// id to next glyph
};

class TEXT_RENDER {
private:
	std::string text;
	GLfloat scale;
	vec3 color;
	vec2 pos;
	GLuint vao, vbo, program; // program: Shader program to draw the font.
	std::map <GLchar, CHARACTER> chars; // map: It is analog to dictionaries in python: structure CHARACTER is the key: store all the loaded characters and assign each GLchar to a character struct in the map.
public:
	TEXT_RENDER(std::string text, std::string font, int size, glm::vec3 color, GLuint program) {
		this->text = text;
		this->color = color;
		this->scale = 1.0;
		this->program = program;
		this->setPosition(pos);

		// ******* Set the shader **********
		glUseProgram(program);

		// ******* Set Projection matrix **********		
		mat4 projection = ortho( // orthographic projection don't have depth
			0.0f, // left
			static_cast<GLfloat>(800), // right: width of the window
			0.0f, // bottom
			static_cast<GLfloat>(600) // top: height of the window
		);
		glUniformMatrix4fv(
			glGetUniformLocation(program, "projection"), // "projection" in Assets/Shaders/TEXT.vs
			1, // passing one matrix
			GL_FALSE, // No need to be transposed
			value_ptr(projection) // Pointer to the data
		);

		// ******* Set Font **********
		FT_Library freeTypeLibrary;
		if (FT_Init_FreeType(&freeTypeLibrary))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		FT_Face fontFace;
		if (FT_New_Face(freeTypeLibrary, font.c_str(), 0, &fontFace))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		FT_Set_Pixel_Sizes(fontFace, 0, size);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction: the font will be drawn jumbled

		// ******* Load 128 ASCII characters **********
		for (GLubyte n = 0; n < 128; n++) {
			if (FT_Load_Char(fontFace, n, FT_LOAD_RENDER)) { // Load character glyph 
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			// Set texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				fontFace->glyph->bitmap.width,
				fontFace->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				fontFace->glyph->bitmap.buffer
			);

			// Set texture filtering options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Create a character
			CHARACTER character = {
				texture,
				glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
				glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
				fontFace->glyph->advance.x
			};

			// Store character in characters map
			chars.insert(std::pair<GLchar, CHARACTER>(n, character));
		}
		// ******* Unbind the texture and destroy fontFace and freeTypeLibrary **********
		glBindTexture(GL_TEXTURE_2D, 0);
		FT_Done_Face(fontFace);
		FT_Done_FreeType(freeTypeLibrary);

		// Each character will be drawn as a texture on a separate quad:

		// ******* Set Vertex Array Object for texture quads **********
		glGenVertexArrays(1, &vao); // Generate. 1: one VAO.
		glBindVertexArray(vao);

		// ******* Set Vertex Buffer Object for texture quads **********
		glGenBuffers(1, &vbo); // Generate
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData( // Bind the data to the buffer
			GL_ARRAY_BUFFER, // Buffer type
			sizeof(GLfloat) * 6 * 4,  // Size in bytes of the buffer data.
			NULL, // Pointer to the data
			GL_DYNAMIC_DRAW // Usage of the data
		);

		// ******* Set position attribute of Vertex **********
		glEnableVertexAttribArray(0); // First and only attribute in: Assets/Shaders/TEXT.vs
		glVertexAttribPointer(
			0,  // index of pos
			4,  // x, y, z and ...?
			GL_FLOAT, 
			GL_FALSE,  // No normalized data
			4 * sizeof(GLfloat),  // Stride: Size of each VERTEX.
			0 // Offset of the pos in each VERTEX.
		);

		// ******* Unbind the buffer and Vertex Array as a precaution **********
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	~TEXT_RENDER() {

	}
	void draw() {

	}
	void setPosition(glm::vec2 _position) {

	}
	void setText(std::string _text) { // set a new string to draw if needed.

	}

};
