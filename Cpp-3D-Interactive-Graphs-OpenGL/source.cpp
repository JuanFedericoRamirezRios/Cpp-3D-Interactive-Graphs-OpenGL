/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/
#include <GL/glew.h> // OpenGL Extension Wrangler Library: OpenGL header files <- GPU.
#include <GLFW/glfw3.h> // Graphics Library Framework: Windows, inputs, events, etc.
#include <btBulletDynamicsCommon.h> // Physics engine.
#include <chrono> // Library of C++ to obtain current time -> Physic, animations.


#include "SHADER_LOADER.hpp"
#include "CAMERA.hpp"
#include "LIGHT_RENDER.hpp"
#include "MESH_RENDER.hpp"
#include "TEXTURE_LOADER.hpp"



using namespace glm;

CAMERA* camera;
LIGHT_RENDER* render;
MESH_RENDER* uvSphere;
MESH_RENDER* ground;

btDiscreteDynamicsWorld* dynamicsWorld; // Track of all the physics.

void InitGame();

void RenderScene(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

static void GlfwError(int id, const char* description) {
	std::cerr << "GLFW Error: " << description << std::endl;
}

int main(int argc, char** argv) {
	glfwSetErrorCallback(&GlfwError);

	glfwInit();
	
	GLFWwindow* window = glfwCreateWindow(800, 600, " Hell world ", NULL, NULL);
	
	glfwMakeContextCurrent(window);
	
	glewInit();

	InitGame();

	auto t0 = std::chrono::high_resolution_clock::
		now();
		
	while (!glfwWindowShouldClose(window)) {
		auto t = std::chrono::high_resolution_clock::
			now();
		float dt = std::chrono::duration<float,	std::chrono::seconds::period> (
				t - t0
			).count();
		dynamicsWorld->stepSimulation(dt);
		
		// Render the scene
		glfwSwapBuffers(window); // Update frames in the buffer by OpenGL. 
		RenderScene(1.0, 1.0, 0.0, 1.0); // Background: Yellow
		glfwPollEvents(); // Check for any events. Ex. close window.

		t0 = t;		
	}
	glfwTerminate();

	delete camera;
	delete render;

	return 0;

}
void InitGame() {
	glEnable(GL_DEPTH_TEST); // GL_DEPTH_TEST: Depth texting -> only the pixels in the front are drawn

	// ******** Set shaders *********
	SHADER_LOADER shaderLoader;
	GLuint flatShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/FLAT_MODEL.vs", "Assets/Shaders/FLAT_MODEL.fs");
	GLuint textureShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/TEXTURE_MODEL.vs", "Assets/Shaders/TEXTURE_MODEL.fs");
	
	// ******** Set camera *********
	camera = new CAMERA(45.0f, 800, 600, 0.1f, 100.0f, vec3(0.0f, 4.0f, 20.0f)); // 800x600: size of window

	// ******** Example tringle mesh without textures *********
	render = new LIGHT_RENDER(MESH_TYPE::Triangle, camera);
	render->SetProgram(flatShaderProgram);
	render->SetPositon(vec3(0.0f, 0.0f, 0.0f)); // triangle in the center of the world

	// ******** Texture loader *********
	TEXTURE_LOADER textureLoader;
	//GLuint sphereTexture = textureLoader.GetTextureID("Assets/Textures/globe.jpg");

	// ******** Load physics *********
	btBroadphaseInterface* broadPhaseCollision = new btDbvtBroadphase(); // broadphase: Using bounding boxes of the objects
	btDefaultCollisionConfiguration* defalultCollisionConf = new btDefaultCollisionConfiguration(); // Default memory.
	btCollisionDispatcher* dispatcherCollision = new	btCollisionDispatcher(defalultCollisionConf); // details of the collision detection, such as which object collided with which other object.
	btSequentialImpulseConstraintSolver* constrains = new btSequentialImpulseConstraintSolver(); // can restrict the motion or rotation
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcherCollision, broadPhaseCollision, constrains, defalultCollisionConf);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
	
	// ******** Sphere rigid body *********
	btCollisionShape* sphereCollisionShape = new btSphereShape(1.0f); // narrowphase: Collision at sphere shape level. radius = 1.
	btDefaultMotionState* sphereMotionState = new btDefaultMotionState(
		btTransform(
			btQuaternion(0, 0, 0, 1), // rotation: (0, 0, 0, 1) -> No rotation. See matrix rotation.
			btVector3(0, 10.0f, 0) // position: (0, 10.0f, 0): 10 along y axis
		)
	);
	btScalar mass = 10.0;
	btVector3 sphereInertia(0, 0, 0);
	sphereCollisionShape->calculateLocalInertia(mass, sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyInfo(mass, sphereMotionState, sphereCollisionShape, sphereInertia);
	btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyInfo);
	sphereRigidBody->setRestitution(1.0f); // 1 <- max value: Rough
	sphereRigidBody->setFriction(1.0f); 
	dynamicsWorld->addRigidBody(sphereRigidBody);

	// ******** Sphere mesh *********
	GLuint sphereTexture = textureLoader.GetTextureID("Assets/Textures/globe.jpg");
	uvSphere = new MESH_RENDER(MESH_TYPE::UVsphere, camera, sphereRigidBody);
	uvSphere->SetProgram(textureShaderProgram);
	uvSphere->SetTexture(sphereTexture);
	//uvSphere->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f)); // Without physics.
	uvSphere->SetScale(vec3(1.0f));

	// ******** Ground rigid body *********
	btCollisionShape* groundCollisionShape = new  btBoxShape(btVector3(4.0f, 0.5f, 4.0f)); // narrowphase: Collision at box shape level. length, height, depth = 4x5x4
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(
		btTransform(
			btQuaternion(0, 0, 0, 1), // rotation: (0, 0, 0, 1) -> No rotation. See matrix rotation.
			btVector3(0, -2.0f, 0) // position: (0, -2.0f, 0): -2 along y axis
		)
	);
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyInfo(
		0.0f, // mass
		groundMotionState, // new btDefaultMotionState(),
		groundCollisionShape, 
		btVector3(0, 0, 0) // inertia
	);
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyInfo);
	groundRigidBody->setRestitution(0.9f); // 1 <- max value: Rough
	groundRigidBody->setFriction(1.0f);
	groundRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT); // will be like a brick wall and won't move and get affected by forces from other rigid	bodies, but other bodies will be affected by it.
	dynamicsWorld->addRigidBody(groundRigidBody);

	// ******** Ground mesh *********
	GLuint groundTexture = textureLoader.GetTextureID("Assets/Textures/ground.jpg");
	ground = new MESH_RENDER(MESH_TYPE::Cube, camera, groundRigidBody);
	ground->SetProgram(textureShaderProgram);
	ground->SetTexture(groundTexture);
	ground->SetScale(vec3(4.0f, 0.5f, 4.0f));
}
void RenderScene(GLclampf red = 0.0, GLclampf green = 0.0, GLclampf blue = 0.0, GLclampf alpha = 1.0) { // Clampled 32 bits float, clamped to the range [0, 1]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer and the depth buffer (if a pixel is behind another pixel, then that pixel will not be stored and show).
	
	glClearColor(red, green, blue, alpha); // Red.  The buffers need to be cleared in every frame.
	
	// Draw game objects
	//render->Draw();
	uvSphere->Draw();
	ground->Draw();
}
