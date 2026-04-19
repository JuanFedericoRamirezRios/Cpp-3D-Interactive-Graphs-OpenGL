/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/

#include <GL/glew.h> // OpenGL Extension Wrangler Library: OpenGL header files <- GPU.
#include "Dependencies/glm/glm/glm.hpp" // OpenGL maths.
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"

using namespace glm;

class CAMERA {
private:
	mat4 viewMatrix;
	mat4 projectionMatrix;
	vec3 pos;
public:
	CAMERA(GLfloat FOV, GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, vec3 pos) { // width and height of viewport.
		this->pos = pos;
		vec3 cameraFront = vec3(0.0f, 0.0f, 0.0f); // looking toward the center of the world coordinates
		vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f); // vector will always be pointing toward the positive y axis

		viewMatrix = lookAt(this->pos, cameraFront, cameraUp);
		projectionMatrix = perspective(FOV, width / height, nearPlane, farPlane); // width / height: Aspect ratio.
	}
	~CAMERA() {

	}
	mat4 GetViewMatrix() {
		return viewMatrix;
	}
	mat4 GetProjectionMatrix() {
		return projectionMatrix;
	}
	vec3 GetPosition() {
		return pos;
	}
};