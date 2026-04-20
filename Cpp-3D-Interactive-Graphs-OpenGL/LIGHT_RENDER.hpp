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

#include "MESH.hpp" // Shape of the light?
#include "SHADER.hpp"
#include "CAMERA.hpp" // Get location, view and projection matrices.

using namespace glm;

class LIGHT_RENDER {
private:
	CAMERA* camera;
	std::vector<VERTEX> vertices;
	std::vector<GLuint> indices;
	vec3 position;
	vec4 color;
	GLuint vbo, ebo, vao, program; 
	/*
	VBO: Vertex Buffer Object: Pos, color, normal, textureCoord. per vertex. -> GPU
	EBO: Element Buffer Object: Store the index of each vertex
	VAO: Vertex Array Object: VBOs and attribures per object
	*/
public:
	LIGHT_RENDER(MESH_TYPE meshType, CAMERA* camera) {
		this->camera = camera;
		// ******* Set Vertex Array Object **********
		glGenVertexArrays(1, &vao); // Generate. 1: one VAO.
		glBindVertexArray(vao); // Binding

		// ******* Load Vertex data **********
		switch (meshType) {
		case Triangle:
			MESH::LoadTriangleVertices(vertices, indices);
			break;
		case Quad:
			MESH::LoadQuadVertices(vertices, indices);
			break;
		case Cube:
			MESH::LoadCubeVertices(vertices, indices);
			break;
		case Sph:
			MESH::LoadSphVertices(vertices, indices);
			break;
		}
		//glGenVertexArrays(1, &vao); // 1: one VAO.
		//glBindVertexArray(vao);


		// ******* Set Vertex Buffer object **********
		glGenBuffers(1, &vbo); // Generate
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // Binding
		glBufferData( // Bind the data to the buffer
			GL_ARRAY_BUFFER, // Buffer type
			sizeof(VERTEX)*vertices.size(), // Size in bytes of the buffer data.
			&vertices[0], // Pointer to the data
			GL_STATIC_DRAW // Usage of the data: Modify the data once and use it many times. 
		);
		
		// Of struct VERTEX, we use pos and color, no normal:
		glEnableVertexAttribArray(0); // position of pos in the Vertex Shader file: Assets/Shaders/FLAT_MODEL.vs
		glVertexAttribPointer(
			0, // index of pos
			3, // x, y and z
			GL_FLOAT, 
			GL_FALSE, // No normalized data
			sizeof(VERTEX), // Stride: Size of each VERTEX.
			(GLvoid*)0 // Offset of the pos in each VERTEX.
		);
		glEnableVertexAttribArray(1); // position of color in the Vertex Shader file: Assets/Shaders/FLAT_MODEL.vs
		glVertexAttribPointer(
			1, // index of color. 0: normal, 1: color.
			4, // r, g, b, alpha
			GL_FLOAT,
			GL_FALSE,
			sizeof(VERTEX),
			(void*)(offsetof(VERTEX, VERTEX::color)) // Offset of the color in each VERTEX.
		);

		// ******* Set Element Buffer object **********
		glGenBuffers(1, &ebo); // Generate
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // Binding. GL_ELEMENT_ARRAY_BUFFER -> index data
		glBufferData( // Bind the data to the buffer
			GL_ELEMENT_ARRAY_BUFFER, // Buffer type
			sizeof(GLuint) * indices.size(), // Size in bytes of the buffer data.
			&indices[0], // Pointer to the data
			GL_STATIC_DRAW // Usage of the data: Modify the data once and use it many times. 
		);

		// ******* Unbind the buffer and Vertex Array as a precaution **********
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
	~LIGHT_RENDER() {

	}
	void Draw() {
		// ******* Set the shader **********
		glUseProgram(program);

		// ******* Set the Model matrix **********
		mat4 model = mat4(1.0f); // model = World matrix = Identity matrix
		model = translate(mat4(1.0), position); // translate the object to the required position

		//glUseProgram(program);

		GLint modelLoc = glGetUniformLocation(program, "model"); // Get the location of the uniform data type of shader.
		glUniformMatrix4fv(
			modelLoc,
			1, // passing one matrix
			GL_FALSE, // No need to be transposed
			value_ptr(model) // Pointer to the data
		);
		
		// ******* Set the View matrix **********
		mat4 view = camera->GetViewMatrix();
		GLint vLoc = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(vLoc, 1, GL_FALSE, value_ptr(view));

		// ******* Set the Projection matrix **********
		mat4 proj = camera->GetProjectionMatrix();
		GLint pLoc = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// ******* Draw the object **********
		glBindVertexArray(vao);
		glDrawElements(
			GL_TRIANGLES, // Mode: GL_LINES | GL_TRIANGLES
			indices.size(), // number of elements or the number of indices
			GL_UNSIGNED_INT, //  type of index data
			0 // location where the indices are stored
		);

		// ******* Unbind the vertex array and the program **********
		glBindVertexArray(0); 
		glUseProgram(0); 
		//glUseProgram(0);
	}
	void SetPositon(vec3 position) {
		this->position = position;
	}
	void SetColor(vec4 color) {
		this->color = color;
	}
	void SetProgram(GLuint program) {
		this->program = program;
	}
	vec3 GetPosition() {
		return position;
	}
	vec4 GetColor() {
		return color;
	}
};


