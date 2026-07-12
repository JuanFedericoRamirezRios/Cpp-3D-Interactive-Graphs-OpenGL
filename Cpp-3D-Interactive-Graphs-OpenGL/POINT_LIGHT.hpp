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

using namespace glm;

class POINT_LIGHT {
private:
	vec3 pos;
	vec4 color;
public:
	POINT_LIGHT(vec3 position, vec4 color) {
		pos = position;
		this->color = color;
	}
	~POINT_LIGHT() {

	}
	void SetPositon(vec3 position) {
		this->pos = position;
	}
	void SetColor(vec4 color) {
		this->color = color;
	}
	vec3 GetPosition() {
		return pos;
	}
	vec4 GetColor() {
		return color;
	}
};