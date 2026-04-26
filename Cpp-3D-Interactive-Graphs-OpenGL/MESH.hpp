#pragma once

/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/
#include <vector>
#include "Dependencies/glm/glm/glm.hpp" // OpenGL maths.

using namespace glm;

enum MESH_TYPE {
	Triangle = 0,
	Quad = 1,
	Cube = 2,
	Sphere = 3
};
struct VERTEX {
	vec3 pos;
	vec3 normal; // -> lighting.
	vec4 color;
	vec2 textureCoord; // -> texture of objects: U and V coordinates?
};

class MESH {
private:

public:
	static void LoadTriangleVertices(std::vector<VERTEX>& vertices, std::vector<uint32_t>& indices) {
		std::vector<VERTEX> _vertices = {
			{ // Vertex 1.
				{0.0f, -1.0f, 0.0f}, // Position
				{0.0f, 0.0f, 1.0f}, // Normal
				{1.0f, 0.0f, 0.0f, 1.0f}, // Color
				{0.0f, 1.0f}, // Texture coordinate
			},
			{ // Vertex 2.
				{1.0f, 1.0f, 0.0f}, // Position
				{0.0f, 0.0f, 1.0f}, // Normal
				{0.0f, 1.0f, 0.0f, 1.0f}, // Color
				{0.0f, 0.0f}, // Texture coordinate
			},
			{ // Vertex 3.
				{-1.0f, 1.0f, 0.0f}, // Position
				{0.0f, 0.0f, 1.0f}, // Normal
				{0.0f, 0.0f, 1.0f, 1.0f}, // Color
				{1.0f, 0.0f}, // Texture coordinate
			}
		};
		std::vector<uint32_t> _indices = { 0, 1, 2 };

		vertices.clear(); indices.clear();
		vertices = _vertices;
		indices = _indices;
	}
	static void LoadQuadVertices(std::vector<VERTEX>& vertices, std::vector<uint32_t>& indices) {
		std::vector<VERTEX> _vertices = {
		{ 
			{ -1.0f, -1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0 },
			{ 1.0f, 0.0f, 0.0f, 1.0f},
			{ 0.0, 1.0 } 
		},
		{ 
			{ -1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 1.0f, 0.0f, 1.0f},
			{ 0.0, 0.0 } 
		},
		{ 
			{ 1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 0.0f, 1.0f, 1.0f},
			{ 1.0, 0.0 } 
		},
		{ 
			{ 1.0f, -1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0 },
			{ 1.0f, 0.0f, 1.0f, 1.0f},
			{ 1.0, 1.0 } 
		}
		};

		std::vector<uint32_t> _indices = {
			0, 1, 2, // The first triangle of mesh.
			0, 2, 3 // The second triangle of mesh.
		};

		vertices.clear(); indices.clear();
		vertices = _vertices;
		indices = _indices;
	}
	static void LoadCubeVertices(std::vector<VERTEX>& vertices, std::vector<uint32_t>& indices) {
		std::vector<VERTEX> _vertices = {
			//front
			{ { -1.0f, -1.0f, 1.0f },{ 0.0f, 0.0f, 1.0 },{ 1.0f, 0.0f, 0.0f, 1.0f},{ 0.0, 1.0 } }, //0
			{ { -1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0 },{ 0.0f, 1.0f, 0.0f, 1.0f},{ 0.0, 0.0 } }, //1
			{ { 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 0.0 } }, //2
			{ { 1.0f, -1.0f, 1.0f },{ 0.0f, 0.0f, 1.0 },{ 1.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 1.0 } }, //3
			// back 
			{ { 1.0, -1.0, -1.0 },{ 0.0f, 0.0f, -1.0 },{ 1.0f, 0.0f, 1.0f, 1.0f},{ 0.0, 1.0 } }, //4
			{ { 1.0f, 1.0, -1.0 },{ 0.0f, 0.0f, -1.0 },{ 0.0f, 1.0f, 1.0f, 1.0f},{ 0.0, 0.0 } }, //5
			{ { -1.0, 1.0, -1.0 },{ 0.0f, 0.0f, -1.0 },{ 0.0f, 1.0f, 1.0f, 1.0f},{ 1.0, 0.0 } }, //6
			{ { -1.0, -1.0, -1.0 },{ 0.0f, 0.0f, -1.0 },{ 1.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 1.0 } }, //7
			//left
			{ { -1.0, -1.0, -1.0 },{ -1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 0.0, 1.0 } }, //8
			{ { -1.0f, 1.0, -1.0 },{ -1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 0.0, 0.0 } }, //9
			{ { -1.0, 1.0, 1.0 },{ -1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 0.0 } },   //10
			{ { -1.0, -1.0, 1.0 },{ -1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 1.0 } }, //11
			//right
			{ { 1.0, -1.0, 1.0 },{ 1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 0.0, 1.0 } }, // 12
			{ { 1.0f, 1.0, 1.0 },{ 1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 0.0, 0.0 } }, //13
			{ { 1.0, 1.0, -1.0 },{ 1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 0.0 } }, //14
			{ { 1.0, -1.0, -1.0 },{ 1.0f, 0.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 1.0 } }, //15
			//top
			{ { -1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 0.0, 1.0 } }, //16
			{ { -1.0f, 1.0f, -1.0f },{ 0.0f, 1.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 0.0, 0.0 } }, //17
			{ { 1.0f, 1.0f, -1.0f },{ 0.0f, 1.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 0.0 } }, //18
			{ { 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 1.0 } }, //19
			//bottom 
			{ { -1.0f, -1.0, -1.0 },{ 0.0f, -1.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 0.0, 1.0 } }, //20
			{ { -1.0, -1.0, 1.0 },{ 0.0f, -1.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 0.0, 0.0 } }, //21
			{ { 1.0, -1.0, 1.0 },{ 0.0f, -1.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 0.0 } },  //22
			{ { 1.0, -1.0, -1.0 },{ 0.0f, -1.0f, 0.0 },{ 0.0f, 0.0f, 1.0f, 1.0f},{ 1.0, 1.0 } }, //23
		};

		std::vector<uint32_t> _indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			4, 6, 7,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23
		};

		vertices.clear(); indices.clear();
		vertices = _vertices;
		indices = _indices;
	}
	static void LoadSphereVertices(std::vector<VERTEX>& vertices, std::vector<uint32_t>& indices) {
		std::vector<VERTEX> _vertices;
		std::vector<uint32_t> _indices;

		float latitudeBands = 20.0f;
		float longitudeBands = 20.0f;
		float radius = 1.0f;

		for (float latNumber = 0; latNumber <= latitudeBands; latNumber++) {
			float theta = latNumber * 3.14f / latitudeBands;
			float sinTheta = sin(theta);
			float cosTheta = cos(theta);

			for (float longNumber = 0; longNumber <= longitudeBands; longNumber++) {

				float phi = longNumber * 2.0f * 3.147f / longitudeBands;
				float sinPhi = sin(phi);
				float cosPhi = cos(phi);

				VERTEX vs;

				vs.textureCoord.x = (longNumber / longitudeBands); // u
				vs.textureCoord.y = (latNumber / latitudeBands);   // v

				vs.normal.x = cosPhi * sinTheta;   // normal x
				vs.normal.y = cosTheta;            // normal y
				vs.normal.z = sinPhi * sinTheta;   // normal z

				vs.color.r = vs.normal.x;
				vs.color.g = vs.normal.y;
				vs.color.b = vs.normal.z;
				vs.color.a = 1.0f;

				vs.pos.x = radius * vs.normal.x; // x
				vs.pos.y = radius * vs.normal.y; // y
				vs.pos.z = radius * vs.normal.z; // z

				_vertices.push_back(vs);
			}
		}

		for (uint32_t latNumber = 0; latNumber < latitudeBands; latNumber++) {
			for (uint32_t longNumber = 0; longNumber < longitudeBands; longNumber++) {
				uint32_t first = (latNumber * ((uint32_t)longitudeBands + 1)) + longNumber;
				uint32_t second = first + (uint32_t)longitudeBands + 1;

				_indices.push_back(first);
				_indices.push_back(second);
				_indices.push_back(first + 1);

				_indices.push_back(second);
				_indices.push_back(second + 1);
				_indices.push_back(first + 1);
			}
		}

		vertices.clear(); indices.clear();
		vertices = _vertices;
		indices = _indices;
	}
};