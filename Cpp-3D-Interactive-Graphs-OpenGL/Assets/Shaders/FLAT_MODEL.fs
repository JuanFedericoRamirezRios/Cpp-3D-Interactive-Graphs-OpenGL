/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/


// ******* This is a Fragment Shader file **********

#version 450 core // GLSL version -> 4.50 OpenGL version

out vec4 color;

in vec4 outColor;


void main(){
    // color = vec4(outColor, 1.0f);
    color = outColor;
}