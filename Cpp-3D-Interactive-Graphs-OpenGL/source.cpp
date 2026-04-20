/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/
#include <GL/glew.h> // OpenGL Extension Wrangler Library: OpenGL header files <- GPU.
#include <GLFW/glfw3.h> // Graphics Library Framework: Windows, inputs, events, etc.

#include "SHADER.hpp"
#include "CAMERA.hpp"
#include "LIGHT_RENDER.hpp"

using namespace glm;

CAMERA* camera;
LIGHT_RENDER* render;

void InitGame();

void RenderScene(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);



int main(int argc, char** argv) {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, " Hell world ", NULL, NULL);

	glfwMakeContextCurrent(window);

	glewInit();

	while (!glfwWindowShouldClose(window)) {
		// Render the scene
		glfwSwapBuffers(window); // Update frames in the buffer by OpenGL.
		RenderScene(1.0, 1.0, 0.0, 1.0); // Background: Yellow
		glfwPollEvents(); // Check for any events. Ex. close window.
	}
	glfwTerminate();

	delete camera;
	delete render;

	return 0;

}
void InitGame() {
	glEnable(GL_DEPTH_TEST); // GL_DEPTH_TEST: Depth texting -> only the pixels in the front are drawn

	SHADER shader;
	GLuint flatShaderProgram = shader.CreateProgram("Assets/Shaders/FLAT_MODEL.vs", "Assets/Shaders/FLAT_MODEL.fs");

	camera = new CAMERA(45.0f, 800, 600, 0.1f, 100.0f, vec3(0.0f, 4.0f, 6.0f)); // 800x600: size of window

	render = new LIGHT_RENDER(MESH_TYPE::Triangle, camera);
	render->SetProgram(flatShaderProgram);
	render->SetPositon(vec3(0.0f, 0.0f, 0.0f)); // triangle in the center of the world?
}
void RenderScene(GLclampf red = 0.0, GLclampf green = 0.0, GLclampf blue = 0.0, GLclampf alpha = 1.0) { // Clampled 32 bits float, clamped to the range [0, 1]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer and the depth buffer (if a pixel is behind another pixel, then that pixel will not be stored and show).
	glClearColor(red, green, blue, alpha); // Red.  The buffers need to be cleared in every frame.

	// Draw game objects
	render->Draw();

}
