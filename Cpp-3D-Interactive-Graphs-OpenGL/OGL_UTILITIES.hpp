#pragma once

/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/
#include <GL/glew.h> // OpenGL Extension Wrangler Library: OpenGL header files <- GPU.
#include <GLFW/glfw3.h> // Graphics Library Framework: Windows, inputs, events, etc.
#include "Dependencies/glm/glm/glm.hpp" // OpenGL maths.
#include "Dependencies/glm/glm/gtc/type_ptr.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"

#include <vector>
#include <btBulletDynamicsCommon.h>
#include <iostream>

#include "CAMERA.hpp" // Get location, view and projection matrices.
#include "POINT_LIGHT.hpp"
#include "MESH_LOADER.hpp"

using namespace glm;

namespace OGL_FEDE {
	void GlfwError(int id, const char* description) {
		std::cerr << "GLFW Error: " << description << std::endl;
	}

	GLFWwindow* InitWindow(int width, int height, const char* title = "Window") {
		glfwSetErrorCallback(&GlfwError);
		glfwInit();
		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(window);
		glewInit();
		return window;
	}

	btDiscreteDynamicsWorld* PhysicsWorld(btVector3 gravity) {
		btBroadphaseInterface* broadPhaseCollision = new btDbvtBroadphase(); // broadphase: Using bounding boxes of the objects
		btDefaultCollisionConfiguration* defalultCollisionConf = new btDefaultCollisionConfiguration(); // Default memory.
		btCollisionDispatcher* dispatcherCollision = new btCollisionDispatcher(defalultCollisionConf); // details of the collision detection, such as which object collided with which other object.
		btSequentialImpulseConstraintSolver* constrains = new btSequentialImpulseConstraintSolver(); // can restrict the motion or rotation
		btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcherCollision, broadPhaseCollision, constrains, defalultCollisionConf);  // Track of all the physics.
		dynamicsWorld->setGravity(gravity);
		return dynamicsWorld;
	}
	btRigidBody* SetRB(
	btCollisionShape* collisionShape,
	btVector3 position,
	btScalar factorGravity,
	btQuaternion rotation,
	btVector3 inertia,
	btScalar restitution,
	btScalar friction
	) {
		collisionShape->calculateLocalInertia(factorGravity, inertia);
		btDefaultMotionState* motionState = new btDefaultMotionState(
			btTransform(
				rotation,
				position
			)
		);
		btRigidBody::btRigidBodyConstructionInfo bodyInfo(
			factorGravity,
			motionState,
			collisionShape,
			inertia
		);
		btRigidBody* rigidBody = new btRigidBody(bodyInfo);
		rigidBody->setRestitution(restitution); // 1 <- max value: Rough
		rigidBody->setFriction(friction);
		return rigidBody;
	}
	
	btRigidBody* SphereRB(
	btScalar radius = 1.0f, 
	btVector3 position = btVector3(0.0f, 0.0f, 0.0f), 
	btScalar factorGravity = 1.0f, 
	btQuaternion rotation = btQuaternion(0, 0, 0, 1), // rotation: (0, 0, 0, 1) -> No rotation. See matrix rotation.
	btVector3 inertia = btVector3(0, 0, 0),
	btScalar restitution = 0.0f,
	btScalar friction = 1.0f
	) {
		btCollisionShape* sphereCollisionShape = new btSphereShape(radius); // narrowphase: Collision at sphere shape level.

		btRigidBody* rigidBody = SetRB(
			sphereCollisionShape,
			position,
			factorGravity,
			rotation,
			inertia,
			restitution,
			friction
		);
		return rigidBody;
	}

	btRigidBody* BoxRB(
	btVector3 size = btVector3(1.0f, 1.0f, 1.0f),
	btVector3 position = btVector3(0.0f, 0.0f, 0.0f),
	btScalar factorGravity = 1.0f,
	btQuaternion rotation = btQuaternion(0, 0, 0, 1), // rotation: (0, 0, 0, 1) -> No rotation. See matrix rotation.
	btVector3 inertia = btVector3(0, 0, 0),
	btScalar restitution = 0.0f,
	btScalar friction = 1.0f
	) {
		btCollisionShape* boxCollisionShape = new btBoxShape(size); // narrowphase: Collision at box shape level. length, height, depth

		btRigidBody* rigidBody = SetRB(
			boxCollisionShape,
			position,
			factorGravity,
			rotation,
			inertia,
			restitution,
			friction
		);
		return rigidBody;
	}
	
	vec3 TranslateAroundY(float period, float radius, float posY, vec3 positionCurrent, btScalar dt) { // Period and dt in seconds.
		float velAng = 2.0f * 3.14f / period; // rad/seg.
		float ang = atan2(positionCurrent.z, positionCurrent.x) + velAng * dt;
		return vec3(radius * cos(ang), posY, radius * sin(ang));
	}

};