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
#include "POINT_LIGHT.hpp"
#include "MESH_LOADER.hpp"
#include <iostream>

#include <btBulletDynamicsCommon.h>

using namespace glm;

enum RENDER {
	empty = 0,
	onlyColor = 1,
	onlyTexture = 2,
	text = 3,
	textureLit = 4
};

class GAME_OBJECT {
private:
	std::vector<VERTEX> vertices;
	std::vector<GLuint> indices;

	RENDER render = empty;
	

	mat4 model; // model = World matrix
	vec3 pos, scale;
	vec4 color;
	float ambientStrength, specularStrength;
	bool withLight;

	//CAMERA* camera;
	POINT_LIGHT* light;
	
	GLuint vbo, ebo, vao, texture/*, program*/;
	/*
	VBO: Vertex Buffer Object: Pos, color, normal, textureCoord. per vertex. -> GPU
	EBO: Element Buffer Object: Store the index of each vertex
	VAO: Vertex Array Object: VBOs and attribures per object
	*/

	void SetBuffersObject() {
		// ******* Set Vertex Array Object **********
		glGenVertexArrays(1, &vao); // Generate. 1: one VAO.
		glBindVertexArray(vao); // Binding

		// ******* Set Vertex Buffer Object **********
		glGenBuffers(1, &vbo); // Generate
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // Binding
		glBufferData( // Bind the data to the buffer
			GL_ARRAY_BUFFER, // Buffer type
			sizeof(VERTEX) * vertices.size(), // Size in bytes of the buffer data.
			&vertices[0], // Pointer to the data
			GL_STATIC_DRAW // Usage of the data: Modify the data once and use it many times. 
		);

		// ******* Set Element Buffer Object **********
		glGenBuffers(1, &ebo); // Generate
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // Binding. GL_ELEMENT_ARRAY_BUFFER -> index data
		glBufferData( // Bind the data to the buffer
			GL_ELEMENT_ARRAY_BUFFER, // Buffer type
			sizeof(GLuint) * indices.size(), // Size in bytes of the buffer data.
			&indices[0], // Pointer to the data
			GL_STATIC_DRAW // Usage of the data: Modify the data once and use it many times. 
		);
	}

public:
	std::string name = "";

	btRigidBody* rigidBody;

	GAME_OBJECT(std::string name,vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 scale = vec3(1.0f, 1.0f, 1.0f)) {
		this->name = name;
		this->pos = position;
		this->scale = scale;
	}
	~GAME_OBJECT() {

	}
	void SetPosition(vec3 position) {
		this->pos = position;
	}
	void SetScale(vec3 scale) {
		this->scale = scale;
	}
	/*void SetCamera(CAMERA* camera) {
		this->camera = camera;
	}*/
	/*void SetProgram(GLuint program) {
		this->program = program;
	}*/
	void SetVertex(MESH_TYPE meshType) {
		// ******* Load Vertex data **********
		switch (meshType) {
		case Triangle:
			MESH_LOADER::LoadTriangleVertices(vertices, indices);
			break;
		case Quad:
			MESH_LOADER::LoadQuadVertices(vertices, indices);
			break;
		case Cube:
			MESH_LOADER::LoadCubeVertices(vertices, indices);
			break;
		case UVsphere:
			MESH_LOADER::LoadUVSphereVertices(vertices, indices);
			break;
		}
	}
	void SetVertex(std::string filePath) {
		// ******* Load Vertex using a json file **********
	}
	void SetColor(vec4 color) {
		if (render == empty || render == onlyColor) {
			render = onlyColor;
		}
		else {
			std::cerr << "Error: Render defined" << std::endl;
			return;
		}
		
		
		this->color = color;
		for (VERTEX vertex : vertices) {
			vertex.color = color;
		}

		SetBuffersObject();

		// ******* Set Attributes of Vertex **********
		// Of struct VERTEX, we use pos and color, no normal:
		glEnableVertexAttribArray(0); // location = 0 attribute in: Assets/Shaders/FLAT_MODEL.vs
		glVertexAttribPointer(
			0, // index of pos
			3, // x, y and z
			GL_FLOAT,
			GL_FALSE, // No normalized data
			sizeof(VERTEX), // Stride: Size of each VERTEX.
			(GLvoid*)0 // Offset of the pos in each VERTEX.
		);
		glEnableVertexAttribArray(1); // location = 1 attribute in: Assets/Shaders/FLAT_MODEL.vs
		glVertexAttribPointer(
			1, // index of color. 
			4, // r, g, b, alpha
			GL_FLOAT,
			GL_FALSE,
			sizeof(VERTEX),
			(void*)(offsetof(VERTEX, VERTEX::color)) // Offset of the color in each VERTEX.
		);

		// ******* Unbind the buffer and Vertex Array as a precaution **********
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void SetTexture(GLuint texture) {
		if (render == empty || render == onlyTexture) {
			render = onlyTexture;
		}
		else {
			std::cerr << "Error: Render defined" << std::endl;
			return;
		}
		this->texture = texture;

		SetBuffersObject();

		// ******* Set Attributes of Vertex **********
		// We use pos and textureCoord, no normal:
		glEnableVertexAttribArray(0); // location = 0 attribute in: Assets/Shaders/TEXTURE_MODEL.vs
		glVertexAttribPointer(
			0, // index of pos
			3, // x, y and z
			GL_FLOAT,
			GL_FALSE, // No normalized data
			sizeof(VERTEX), // Stride: Size of each VERTEX.
			(GLvoid*)0 // Offset of the pos in each VERTEX.
		);
		glEnableVertexAttribArray(1); // location = 1 attribute in: Assets/Shaders/TEXTURE_MODEL.vs
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
	void SetTextureLit(GLuint texture, POINT_LIGHT* light) {
		if (render == empty || render == textureLit) {
			render = textureLit;
		}
		else {
			std::cerr << "Error: Render defined" << std::endl;
			return;
		}
		this->texture = texture;
		this->light = light;

		SetBuffersObject();

		// ******* Set Attributes of Vertex **********
		// We use pos and textureCoord, no normal:
		glEnableVertexAttribArray(0); // location = 0 attribute in: Assets/Shaders/LIT_TEXTURE_MODEL.vs
		glVertexAttribPointer(
			0, // index of pos
			3, // x, y and z
			GL_FLOAT,
			GL_FALSE, // No normalized data
			sizeof(VERTEX), // Stride: Size of each VERTEX.
			(GLvoid*)0 // Offset of the pos in each VERTEX.
		);
		glEnableVertexAttribArray(1); // location = 1 attribute in: Assets/Shaders/LIT_TEXTURE_MODEL.vs
		glVertexAttribPointer(
			1, // index of textureCoord
			2, // coordinates of texture of objects: U and V coordinates?
			GL_FLOAT,
			GL_FALSE,
			sizeof(VERTEX),
			(void*)(offsetof(VERTEX, VERTEX::textureCoord)) // Offset of the textureCoord in each VERTEX.
		);
		glEnableVertexAttribArray(2); // location = 2 attribute in: Assets/Shaders/LIT_TEXTURE_MODEL.vs
		glVertexAttribPointer(
			2, // index of normal
			3, // coordinates of normal vector
			GL_FLOAT,
			GL_FALSE,
			sizeof(VERTEX),
			(void*)(offsetof(VERTEX, VERTEX::normal)) // Offset of the textureCoord in each VERTEX.
		);
		// ******* Unbind the buffer and Vertex Array as a precaution **********
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
	

	void Draw(GLuint program, CAMERA* camera) {
		if (render == empty) {
			std::cerr << "Error: Render no defined" << std::endl;
			return;
		}

		// ******* Set the shader **********
		glUseProgram(program);

		if (render == onlyColor) {
			// ******* Set the Model matrix **********
			mat4 model = mat4(1.0f); // model = World matrix = Identity matrix
			model = translate(mat4(1.0), pos); // translate the object to the required position
			// Send to the shader
			GLint modelLoc = glGetUniformLocation(program, "model"); // "model" in Assets/Shaders/FLAT_MODEL.vs
			glUniformMatrix4fv(
				modelLoc,
				1, // passing one matrix
				GL_FALSE, // No need to be transposed
				value_ptr(model) // Pointer to the data
			);

			// ******* Set the View matrix **********
			mat4 view = camera->GetViewMatrix();
			// Send to the shader
			GLint vLoc = glGetUniformLocation(program, "view"); // "view" in Assets/Shaders/FLAT_MODEL.vs
			glUniformMatrix4fv(vLoc, 1, GL_FALSE, value_ptr(view));

			// ******* Set the Projection matrix **********
			mat4 proj = camera->GetProjectionMatrix();
			// Send to the shader
			GLint pLoc = glGetUniformLocation(program, "projection"); // "projection" in Assets/Shaders/FLAT_MODEL.vs
			glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(proj));
		}

		

		// ******* Draw the object **********
		glBindVertexArray(vao);
		glDrawElements(
			GL_TRIANGLES, // Mode: GL_LINES | GL_TRIANGLES
			(GLsizei)indices.size(), // number of elements or the number of indices
			GL_UNSIGNED_INT, //  type of index data
			0 // location where the indices are stored
		);

		// ******* Unbind the vertex array and the program **********
		glBindVertexArray(0);
		glUseProgram(0);
	}

	vec3 GetPosition() {
		return pos;
	}
	vec4 GetColor() {
		return color;
	}






};