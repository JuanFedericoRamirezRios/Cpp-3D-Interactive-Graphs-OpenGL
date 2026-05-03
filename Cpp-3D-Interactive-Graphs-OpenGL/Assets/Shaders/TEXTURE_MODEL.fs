/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/


// ******* This is a Fragment Shader file **********

#version 450 core // GLSL version -> 4.50 OpenGL version

in vec2 outTextureCoord; // Out from Assets/Shaders/TEXTURE_MODEL.vs

out vec4 color;

// texture
uniform sampler2D uTexture; // sampler2D: sample the texture <- wrapping and filtering in TEXTURE.hpp

void main(){
    
    color = texture(uTexture, outTextureCoord); // the color is a texture.
}