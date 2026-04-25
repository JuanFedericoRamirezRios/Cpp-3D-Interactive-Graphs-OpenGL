#pragma once

/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/

#include <GL/glew.h> // OpenGL Extension Wrangler Library: OpenGL header files <- GPU.
#include <iostream>
#include <fstream>
#include <vector>

class SHADER { // Used to Vertex Shader and Fragment Shader of OpenGL_pipeline.png
private:
	std::string ReadFileShader(const char* shaderFileName) { // Name of the vertexShaderFile | fragmentShaderFile
		std::string textFileShader;
		std::ifstream file(shaderFileName, std::ios::in);

		if (!file.good()) {
			std::cerr << "Error: No file " << shaderFileName << std::endl;
			std::terminate();
		}

		file.seekg(0, std::ios::end); //Points to the final, sets the get pointer 0 bytes
		textFileShader.resize((unsigned int)file.tellg()); // tellg() -> current position of get pointer

		file.seekg(0, std::ios::beg); //Points to the beginning, sets the get pointer 0 bytes.
		file.read(&textFileShader[0], textFileShader.size());

		file.close();

		return textFileShader;
	}
	GLuint CreateShader(GLenum shaderType, std::string textFileShader, const char* nameTypeShader) { // shaderType: In this case: vertexShader | fragmentShader
		GLuint shader = glCreateShader(shaderType);
		const char* charsFileShader = textFileShader.c_str(); // Conver to char*
		const int sizeFileShader = (int)textFileShader.size();
		glShaderSource(shader, 1, &charsFileShader, &sizeFileShader); // charsFileShader -> shader, 1: One string.
		glCompileShader(shader);

		int compileResult = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult); // Retrieves info, as Integer Vector, about GL_COMPILE_STATUS -> compileResult.
		if (compileResult == GL_FALSE) {
			int infoLogLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

			std::vector<char> infoLog(infoLogLength);
			glGetShaderInfoLog(shader, infoLogLength, NULL, &infoLog[0]);
			std::cerr << "Error compiling shader, Type " << nameTypeShader << ": " << &infoLog[0] << std::endl;

			return 0;
		}
		return shader;
	}
public:
	GLuint CreateProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
		std::string vertexTextFile = ReadFileShader(vertexShaderFileName);
		GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexTextFile, "Vertex Shader");

		std::string fragmentTextFile = ReadFileShader(fragmentShaderFileName);
		GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentTextFile, "Fragment Shader");

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		int linkResult = 0;
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
		if (linkResult == GL_FALSE) {
			int infoLogLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
			
			std::vector<char> infoLog(infoLogLength);
			glGetProgramInfoLog(program, infoLogLength, NULL, &infoLog[0]);
			std::cerr << "Error link loader program: " << &infoLog[0] << std::endl;

			return 0;
		}
		return program;
	}
};