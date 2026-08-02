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
#include "OGL_UTILITIES.hpp"
#include <iostream>

#include <btBulletDynamicsCommon.h>

using namespace glm;

enum RENDER_TYPE {
	empty = 0,
	onlyColor = 1,
	onlyTexture = 2,
	textureLit = 3,
};

class GAME_OBJECT {
private:
	std::vector<VERTEX> vertices;
	std::vector<GLuint> indices;

	//mat4 model; // model = World matrix
	vec3 pos, scale;
	
	RENDER_TYPE renderType = empty;
	bool withRigidBody = false;

	POINT_LIGHT* light;
	float ambientStrength, specularStrength;
	
	GLuint vbo, ebo, vao;
	GLuint texture;
	GLuint program;
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
	vec3 GetPosition() {
		return pos;
	}
	void SetScale(vec3 scale) {
		this->scale = scale;
	}
	vec3 GetScale() {
		return scale;
	}
	void SetRB(btRigidBody* rigidBody) {
		if (withRigidBody) {
			std::cerr << "Error: Object " << name << " can not overwrite the Rigid Body." << std::endl;
			return;
		}
		withRigidBody = true;

		this->rigidBody = rigidBody;
		this->rigidBody->setUserPointer(this); // access the name of the rendered mesh
	}
	void SetSphereRB(
		btScalar radius = 1.0f,
		btScalar factorGravity = 1.0f,
		btQuaternion rotation = btQuaternion(0, 0, 0, 1), // rotation: (0, 0, 0, 1) -> No rotation. See matrix rotation.
		btVector3 inertia = btVector3(0, 0, 0),
		btScalar restitution = 0.0f,
		btScalar friction = 1.0f
	) {
		if (withRigidBody) {
			std::cerr << "Error: Object " << name << " can not overwrite the Rigid Body." << std::endl;
			return;
		}
		withRigidBody = true;

		rigidBody = OGL_FEDE::SphereRB(
			radius,
			btVector3(pos.x, pos.y, pos.z),
			factorGravity,
			rotation, 
			inertia,
			restitution,
			friction
		);
		rigidBody->setUserPointer(this); // access the name of the rendered mesh
	}
	void SetBoxRB(
		btScalar factorGravity = 1.0f,
		btQuaternion rotation = btQuaternion(0, 0, 0, 1), // rotation: (0, 0, 0, 1) -> No rotation. See matrix rotation.
		btVector3 inertia = btVector3(0, 0, 0),
		btScalar restitution = 0.0f,
		btScalar friction = 1.0f
	) {
		if (withRigidBody) {
			std::cerr << "Error: Object " << name << " can not overwrite the Rigid Body." << std::endl;
			return;
		}
		withRigidBody = true;

		rigidBody = OGL_FEDE::BoxRB(
			btVector3(scale.x, scale.y, scale.z),
			btVector3(pos.x, pos.y, pos.z),
			factorGravity,
			rotation,
			inertia,
			restitution,
			friction
		);
		rigidBody->setUserPointer(this); // access the name of the rendered mesh
	}

	void SetProgram(GLuint program) {
		this->program = program;
	}
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
	void SetVertex(std::string filePath="") {
		MESH_LOADER::LoadCubeVertices(vertices, indices);
		// ******* Load Vertex using a json file **********
	}
	void SetDefaultColor(GLuint program) { // Color of VERTEX.
		if (renderType != empty) {
			std::cerr << "Error: Object " << name << " can not overwrite the render." << std::endl;
			return;
		}
		else {
			renderType = onlyColor;
		}
		this->program = program;
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
	void SetColor(GLuint program, vec4 color) {
		if (renderType != empty) {
			std::cerr << "Error: Object " << name << " can not overwrite the render." << std::endl;
			return;
		}
				
		for (VERTEX &vertex : vertices) {
			vertex.color = color;
		}

		SetDefaultColor(program);
		
	}
	vec4 GetColor() {
		return vertices[0].color;
	}
	void SetTexture(GLuint program, GLuint texture) {
		if (renderType != empty) {
			std::cerr << "Error: Object " << name << " can not overwrite the render." << std::endl;
			return;
		}
		else {
			renderType = onlyTexture;
		}
		this->program = program;
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
	void SetTextureLit(GLuint program, GLuint texture, POINT_LIGHT* light, float specularStrength, float ambientStrength) {
		if (renderType != empty) {
			std::cerr << "Error: Object " << name << " can not overwrite the render." << std::endl;
			return;
		}
		else {
			renderType = textureLit;
		}
		this->program = program;
		this->texture = texture;
		this->light = light;
		this->ambientStrength = ambientStrength;
		this->specularStrength = specularStrength;

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

	void Draw(CAMERA* camera) {
		if (renderType == empty) {
			std::cerr << "Error: Object " << name << " do not have render." << std::endl;
			return;
		}

		// ******* Set the shader **********
		glUseProgram(program);

		// ******* Set the Model matrix **********
		mat4 model; // model = World matrix = Identity matrix
		//model = translate(mat4(1.0), pos); // translate the object to the required position}
		mat4 scale = glm::scale(mat4(1.0f), this->scale);
		if (!withRigidBody) { // Without physics:
			//std::cout << "color without RB" << std::endl;
			mat4 translation = translate(mat4(1.0f), pos);
			model = translation * scale;
		}
		else {
			btTransform transformation;
			rigidBody->getMotionState()->getWorldTransform(transformation); // get the transformation from the rigidBody
			btQuaternion rotationQuat = transformation.getRotation();
			btVector3 translateVec = transformation.getOrigin();
			mat4 rotation = glm::rotate(
				mat4(1.0f),
				rotationQuat.getAngle(),
				vec3(
					rotationQuat.getAxis().getX(),
					rotationQuat.getAxis().getY(),
					rotationQuat.getAxis().getZ()
				)
			);
			mat4 translation = glm::translate(
				mat4(1.0f),
				vec3(
					translateVec.getX(),
					translateVec.getY(),
					translateVec.getZ()
				)
			);
			model = translation * rotation * scale;
		}
		// Send to the shader
		GLint modelLoc = glGetUniformLocation(program, "model"); // "model" in Assets/Shaders/FLAT_MODEL.vs, Assets/Shaders/TEXTURE_MODEL.vs or LIT_TEXTURE_MODEL.vs
		glUniformMatrix4fv(
			modelLoc,
			1, // passing one matrix
			GL_FALSE, // No need to be transposed
			value_ptr(model) // Pointer to the data
		);

		if (renderType == onlyColor) {
			
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
		else if ((renderType == onlyTexture) || (renderType == textureLit)) {
			// ******* Set Projection and View matrix **********
			mat4 projectionView = camera->GetProjectionMatrix() * camera->GetViewMatrix();
			// Send to the shader
			GLint vpLoc = glGetUniformLocation(program, "projectionView"); // "projectionView" in Assets/Shaders/TEXTURE_MODEL.vs or LIT_TEXTURE_MODEL.vs
			glUniformMatrix4fv(
				vpLoc,
				1, // passing one matrix
				GL_FALSE, // No need to be transposed
				value_ptr(projectionView) // Pointer to the data
			);

			// ******* Bind the texture **********
			glBindTexture(GL_TEXTURE_2D, texture); // 2D texture

			// ******* Set Lighting **********
			if (renderType == textureLit) {
				GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos"); // in Assets/Shaders/LIT_TEXTURE_MODEL.fs
				glUniform3f(cameraPosLoc, camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);

				GLuint lightPosLoc = glGetUniformLocation(program, "lightPos"); // in Assets/Shaders/LIT_TEXTURE_MODEL.fs
				glUniform3f(lightPosLoc, this->light->GetPosition().x, this->light->GetPosition().y, this->light->GetPosition().z);

				GLuint lightColorLoc = glGetUniformLocation(program, "lightColor"); // in Assets/Shaders/LIT_TEXTURE_MODEL.fs
				glUniform3f(lightColorLoc, this->light->GetColor().x, this->light->GetColor().y, this->light->GetColor().z);

				GLuint specularStrengthLoc = glGetUniformLocation(program, "specularStrength"); // in Assets/Shaders/LIT_TEXTURE_MODEL.fs
				glUniform1f(specularStrengthLoc, specularStrength);

				GLuint ambientStrengthLoc = glGetUniformLocation(program, "ambientStrength"); // in Assets/Shaders/LIT_TEXTURE_MODEL.fs
				glUniform1f(ambientStrengthLoc, ambientStrength);
			}
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
	
	
};