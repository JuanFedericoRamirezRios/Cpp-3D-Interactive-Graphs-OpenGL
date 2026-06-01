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
