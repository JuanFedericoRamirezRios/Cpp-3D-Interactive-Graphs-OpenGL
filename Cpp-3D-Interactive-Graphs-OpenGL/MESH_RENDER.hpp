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
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"

#include <vector>
#include "CAMERA.hpp" // Get location, view and projection matrices.
#include "LIGHT_RENDER.hpp"

class MESH_RENDER {
private:
	std::vector<VERTEX> vertices;
	std::vector<GLuint> indices;
	mat4 model; // model = World matrix

	CAMERA* camera;

	vec3 position, scale;
	GLuint vbo, ebo, vao, texture, program;
	/*
	VBO: Vertex Buffer Object: Pos, color, normal, textureCoord. per vertex. -> GPU
	EBO: Element Buffer Object: Store the index of each vertex
	VAO: Vertex Array Object: VBOs and attribures per object
	*/

public:
	MESH_RENDER(MESH_TYPE meshType, CAMERA* camera) {
		this->camera = camera;
		scale = vec3(1.0f, 1.0f, 1.0f);
		position = vec3(0.0f, 0.0f, 0.0f);

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
		case Sphere:
			MESH::LoadSphereVertices(vertices, indices);
			break;
		}

		// ******* Set Vertex Buffer object **********
		glGenBuffers(1, &vbo); // Generate
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // Binding
		glBufferData( // Bind the data to the buffer
			GL_ARRAY_BUFFER, // Buffer type
			sizeof(VERTEX) * vertices.size(), // Size in bytes of the buffer data.
			&vertices[0], // Pointer to the data
			GL_STATIC_DRAW // Usage of the data: Modify the data once and use it many times. 
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

		// ******* Set Attributes of Vertex **********
		// We use pos and textureCoord, no normal:
		glEnableVertexAttribArray(0); // First attribute in: Assets/Shaders/...
		glVertexAttribPointer(
			0, // index of pos
			3, // x, y and z
			GL_FLOAT,
			GL_FALSE, // No normalized data
			sizeof(VERTEX), // Stride: Size of each VERTEX.
			(GLvoid*)0 // Offset of the pos in each VERTEX.
		);
		glEnableVertexAttribArray(1); // Second attribute in: Assets/Shaders/...
		glVertexAttribPointer(
			1, // index of textureCoord
			2, // coordinates of texture of objects: U and V coordinates?
			GL_FLOAT,
			GL_FALSE,
			sizeof(VERTEX),
			(void*)(offsetof(VERTEX, VERTEX::textureCoord)) // Offset of the textureCoord in each VERTEX.
		);

		// ******* Unbind the buffer and Vertex Array as a precaution **********
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	~MESH_RENDER() {

	}
	void Draw() {

	}
	void SetPosition(vec3 position) {

	}
	void SetScale(vec3 scale) {

	}
	void SetProgram(GLuint program) {

	}
	void SetTexture(GLuint textureID) {

	}

};