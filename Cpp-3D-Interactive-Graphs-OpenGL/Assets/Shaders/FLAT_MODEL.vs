/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/

// ******* This is a Vertex Shader file **********

#version 450 core // GLSL version -> 4.50 OpenGL version
layout (location = 0) in vec3 position; // position -> glEnableVertexAttribArray(0) in LIGHT_RENDER.hpp
layout (location = 1) in vec4 color; // position -> glEnableVertexAttribArray(1) in LIGHT_RENDER.hpp

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model; // model = world

out vec4 outColor; // sent to Assets/Shaders/FLAT_MODEL.fs

void main(void) {
	gl_Position = projection * view * model * vec4(position, 1.0); // GLSL intrinsic variable: Final position.
	outColor = color;
}