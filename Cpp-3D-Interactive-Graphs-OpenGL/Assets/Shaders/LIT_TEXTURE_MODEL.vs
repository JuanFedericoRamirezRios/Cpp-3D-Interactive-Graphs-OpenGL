/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/

// ******* This is a Vertex Shader file **********

#version 450 core // GLSL version -> 4.50 OpenGL version
layout (location = 0) in vec3 position; // position -> glEnableVertexAttribArray(0) in MESH_RENDER.hpp
layout (location = 1) in vec2 textureCoord; // textureCoord -> glEnableVertexAttribArray(1) in MESH_RENDER.hpp
layout (location = 2) in vec3 normal; // textureCoord -> glEnableVertexAttribArray(2) in MESH_RENDER.hpp

out vec2 outTextureCoord; // sent to Assets/Shaders/LIT_TEXTURE_MODEL.fs
out vec3 outNormalVector; // sent to Assets/Shaders/LIT_TEXTURE_MODEL.fs
out vec3 outFragmentWorldPos; // sent to Assets/Shaders/LIT_TEXTURE_MODEL.fs. Position of the pixel in the world: world x localPosition

uniform mat4 projectionView;
uniform mat4 model; // model = world

void main(void) {
	gl_Position = projectionView * model * vec4(position, 1.0); // GLSL intrinsic variable: Final position.
	outTextureCoord = textureCoord;
    outFragmentWorldPos = vec3(model * vec4(position, 1.0));
    outNormalVector = mat3(transpose(inverse(model))) * normal;
}