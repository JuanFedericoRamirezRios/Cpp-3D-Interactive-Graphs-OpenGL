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
	Sph = 3
};
struct VERTEX {
	vec3 pos;
	vec3 normal; // -> lighting.
	vec4 color;
	vec2 textureCoord; // -> texture of objects.
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

	}
	static void LoadCubeVertices(std::vector<VERTEX>& vertices, std::vector<uint32_t>& indices) {

	}
	static void LoadSphVertices(std::vector<VERTEX>& vertices, std::vector<uint32_t>& indices) {

	}
};