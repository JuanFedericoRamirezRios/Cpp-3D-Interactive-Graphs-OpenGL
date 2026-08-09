#pragma once

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
	GLfloat FOV;
	GLfloat width;
	GLfloat height;
	GLfloat nearPlane; 
	GLfloat farPlane;
	vec3 pos;
	vec3 cameraFront;
	vec3 cameraUp;
	
public:
	CAMERA(GLfloat FOV, GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, vec3 position, vec3 cameraFront = vec3(0.0f, 0.0f, 0.0f), vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f)) { // width and height of viewport.
		//cameraFront = vec3(0.0f, 0.0f, 0.0f); // looking toward the center of the world coordinates.
		//cameraUp = vec3(0.0f, 1.0f, 0.0f); // vector will always be pointing toward the positive y axis.
		this->FOV = FOV;
		this->width = width;
		this->height = height;
		this->nearPlane = nearPlane;
		this->farPlane = farPlane;
		this->pos = position;
		this->cameraFront = cameraFront;
		this->cameraUp = cameraUp;
		SetViewProjection();
	}
	void SetViewProjection() {
		viewMatrix = lookAt(pos, cameraFront, cameraUp);
		projectionMatrix = perspective(FOV, width / height, nearPlane, farPlane); // width / height: Aspect ratio.
	}
	void SetFOV(GLfloat FOV) {
		this->FOV = FOV;
		SetViewProjection();
	}
	GLfloat GetFOV() {
		return FOV;
	}
	void SetSize(GLfloat width, GLfloat height) {
		this->width = width;
		this->height = height;
		SetViewProjection();
	}
	void SetPlanes(GLfloat nearPlane, GLfloat farPlane) {
		this->nearPlane = nearPlane;
		this->farPlane = farPlane;
		SetViewProjection();
	}
	void SetPosition(vec3 position) {
		pos = position;
		SetViewProjection();
	}
	vec3 GetPosition() {
		return pos;
	}
	void SetCameras(vec3 cameraFront, vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f)) {
		this->cameraFront = cameraFront;
		SetViewProjection();
	}
	vec3 GetCameraFront() {
		return cameraFront;
	}
	vec3 GetCameraUp() {
		return cameraUp;
	}
	~CAMERA() {

	}
	mat4 GetViewMatrix() {
		return viewMatrix;
	}
	mat4 GetProjectionMatrix() {
		return projectionMatrix;
	}
	
};