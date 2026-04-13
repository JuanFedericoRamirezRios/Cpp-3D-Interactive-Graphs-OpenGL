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
		glGenVertexArrays(1, &vao); // 1: one VAO.
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(
			GL_ARRAY_BUFFER, // Buffer type
			sizeof(VERTEX)*vertices.size(), // Size in bytes of the buffer data.
			&vertices[0], // Pointer to the data
			GL_STATIC_DRAW // Usage of the data: Modify the data once and use it many times. 
		);
		
		// Of struct VERTEX, we use pos and color:
		glEnableVertexAttribArray(0); // pos
		glVertexAttribPointer(
			0, // index of pos
			3, // x, y and z
			GL_FLOAT, 
			GL_FALSE, // No normalized data
			sizeof(VERTEX), // Stride: Size of each VERTEX.
			(GLvoid*)0 // Offset of the pos in each VERTEX.
		);
		glEnableVertexAttribArray(1); // 0: normal, 1: color.
		glVertexAttribPointer(
			1, // index of color
			4, // r, g, b, alpha
			GL_FLOAT,
			GL_FALSE,
			sizeof(VERTEX),
			(void*)(offsetof(VERTEX, VERTEX::color)) // Offset of the color in each VERTEX.
		);

	}
	~LIGHT_RENDER() {

	}
	void Draw() {

	}
	void SetPositon(vec3 position) {

	}
	void SetColor(vec4 color) {

	}
	void SetProgram(GLuint program) {

	}
	vec3 GetPosition() {

	}
	vec4 GetColor() {

	}
};


