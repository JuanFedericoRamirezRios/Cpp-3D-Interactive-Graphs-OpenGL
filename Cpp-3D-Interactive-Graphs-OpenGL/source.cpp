/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/
#include <GL/glew.h> // OpenGL Extension Wrangler Library: OpenGL header files <- GPU.
#include <GLFW/glfw3.h> // Graphics Library Framework: Windows, inputs, events, etc.

void RenderScene(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

int main(int argc, char** argv) {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, " Hell world ", NULL, NULL);

	glfwMakeContextCurrent(window);

	glewInit();

	while (!glfwWindowShouldClose(window)) {
		// Render the scene
		glfwSwapBuffers(window); // Update frames in the buffer by OpenGL.
		RenderScene(1.0, 0.0, 0.0, 1.0);
		glfwPollEvents(); // Check for any events. Ex. close window.
	}
	glfwTerminate();

	return 0;

}
void RenderScene(GLclampf red = 0.0, GLclampf green = 0.0, GLclampf blue = 0.0, GLclampf alpha = 1.0) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer and the depth buffer (if a pixel is behind another pixel, then that pixel will not be stored and show).
	glClearColor(red, green, blue, alpha); // Red.  The buffers need to be cleared in every frame.

	// Draw game objects

}