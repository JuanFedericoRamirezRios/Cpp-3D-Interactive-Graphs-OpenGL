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

class OGL_FEDE {
private:

public:
	static void GlfwError(int id, const char* description) {
		std::cerr << "GLFW Error: " << description << std::endl;
	}
	static GLFWwindow* InitWindow(int width, int height, const char* title = "Window") {
		glfwSetErrorCallback(&GlfwError);
		glfwInit();
		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(window);
		glewInit();
		return window;
	}
	static btDiscreteDynamicsWorld* PhysicsWorld(btVector3 gravity) {
		btBroadphaseInterface* broadPhaseCollision = new btDbvtBroadphase(); // broadphase: Using bounding boxes of the objects
		btDefaultCollisionConfiguration* defalultCollisionConf = new btDefaultCollisionConfiguration(); // Default memory.
		btCollisionDispatcher* dispatcherCollision = new btCollisionDispatcher(defalultCollisionConf); // details of the collision detection, such as which object collided with which other object.
		btSequentialImpulseConstraintSolver* constrains = new btSequentialImpulseConstraintSolver(); // can restrict the motion or rotation
		btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcherCollision, broadPhaseCollision, constrains, defalultCollisionConf);  // Track of all the physics.
		dynamicsWorld->setGravity(gravity);
		return dynamicsWorld;
	}

};