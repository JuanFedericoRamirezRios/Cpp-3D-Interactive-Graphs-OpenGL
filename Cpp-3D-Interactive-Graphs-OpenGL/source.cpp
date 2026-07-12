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
#include "TEXT_RENDER.hpp"

using namespace glm;

CAMERA* camera;
LIGHT_RENDER* light;
MESH_RENDER* uvSphere;
MESH_RENDER* ground;
MESH_RENDER* enemy;
TEXT_RENDER* label;


btDiscreteDynamicsWorld* dynamicsWorld; // Track of all the physics.

GLuint flatShaderProgram, textureShaderProgram, litTextureShaderProgram, textShaderProgram;

GLuint sphereTexture, groundTexture;

bool grounded = false;
bool gameOver = true;
int score = 0;

void InitGame();

void RenderScene(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

static void GlfwError(int id, const char* description) {
	std::cerr << "GLFW Error: " << description << std::endl;
}
void AddRigidBodMesh() {
	// ******** Sphere rigid body *********
	btCollisionShape* sphereCollisionShape = new btSphereShape(1.0f); // narrowphase: Collision at sphere shape level. radius = 1.
	btDefaultMotionState* sphereMotionState = new btDefaultMotionState(
		btTransform(
			btQuaternion(0, 0, 0, 1), // rotation: (0, 0, 0, 1) -> No rotation. See matrix rotation.
			btVector3(0, 0.5f, 0) // position: (0, .0.5f, 0): 0.5 along y axis
		)
	);
	btScalar mass = 13.0;
	btVector3 sphereInertia(0, 0, 0);
	sphereCollisionShape->calculateLocalInertia(mass, sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyInfo(mass, sphereMotionState, sphereCollisionShape, sphereInertia);
	btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyInfo);
	sphereRigidBody->setRestitution(0.0f); // 1 <- max value: Rough
	sphereRigidBody->setFriction(1.0f);
	sphereRigidBody->setActivationState(DISABLE_DEACTIVATION); // We need to control the jump.
	dynamicsWorld->addRigidBody(sphereRigidBody);

	// ******** Sphere mesh *********
	uvSphere = new MESH_RENDER(MESH_TYPE::UVsphere, "hero", camera, sphereRigidBody, light, 0.1f, 0.5f);
	//uvSphere->SetProgram(textureShaderProgram);
	uvSphere->SetProgram(litTextureShaderProgram);
	uvSphere->SetTexture(sphereTexture);
	//uvSphere->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f)); // Without physics.
	uvSphere->SetScale(vec3(1.0f));
	sphereRigidBody->setUserPointer(uvSphere); // access the name of the rendered mesh

	// ******** Ground rigid body *********
	btCollisionShape* groundCollisionShape = new  btBoxShape(btVector3(4.0f, 0.5f, 4.0f)); // narrowphase: Collision at box shape level. length, height, depth = 4x5x4
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(
		btTransform(
			btQuaternion(0, 0, 0, 1), // rotation: (0, 0, 0, 1) -> No rotation. See matrix rotation.
			btVector3(0, -1.0f, 0) // position: (0, -1.0f, 0): -1 along y axis
		)
	);
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyInfo(
		0.0f, // mass
		groundMotionState, // new btDefaultMotionState(),
		groundCollisionShape,
		btVector3(0, 0, 0) // inertia
	);
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyInfo);
	groundRigidBody->setRestitution(0.0f); // 1 <- max value: Rough
	groundRigidBody->setFriction(1.0f);
	groundRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT); // will be like a brick wall and won't move and get affected by forces from other rigid	bodies, but other bodies will be affected by it.
	dynamicsWorld->addRigidBody(groundRigidBody);

	// ******** Ground mesh *********
	ground = new MESH_RENDER(MESH_TYPE::Cube, "ground", camera, groundRigidBody, light, 0.1f, 0.5f);
	//ground->SetProgram(textureShaderProgram);
	ground->SetProgram(litTextureShaderProgram);
	ground->SetTexture(groundTexture);
	ground->SetScale(vec3(4.0f, 0.5f, 4.0f));
	groundRigidBody->setUserPointer(ground);

	// ******** Enemy rigid body *********
	btCollisionShape* cubeCollisionShape = new  btBoxShape(btVector3(1.0f, 1.0f, 1.0f)); // narrowphase: Collision at box shape level.
	btDefaultMotionState* cubeMotionState = new btDefaultMotionState(
		btTransform(
			btQuaternion(0, 0, 0, 1), // rotation: (0, 0, 0, 1) -> No rotation. See matrix rotation.
			btVector3(18.0f, 1.0f, 0) // position: 18 in x and 1 in y
		)
	);
	btRigidBody::btRigidBodyConstructionInfo cubeRigidBodyInfo(
		0.0f, // mass
		cubeMotionState, // new btDefaultMotionState(),
		cubeCollisionShape,
		btVector3(0, 0, 0) // inertia
	);
	btRigidBody* cubeRigidBody = new btRigidBody(cubeRigidBodyInfo);
	cubeRigidBody->setRestitution(0.0f); // 1 <- max value: Rough
	cubeRigidBody->setFriction(1.0f);
	// cubeRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT); // exert force on other	objects.
	cubeRigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE); // CF_NO_CONTACT_RESPONSE: No check if there was an overlap between the enemy rigid body and another body. CF_KINEMATIC_OBJECT: The objects respond to collision.
	dynamicsWorld->addRigidBody(cubeRigidBody);

	// ******** Enemy mesh *********
	// GLuint cubeTexture = textureLoader.GetTextureID("Assets/Textures/ground.jpg");
	enemy = new MESH_RENDER(MESH_TYPE::Cube, "enemy", camera, cubeRigidBody, light, 0.1f, 0.5f);
	//enemy->SetProgram(textureShaderProgram);
	enemy->SetProgram(litTextureShaderProgram);
	enemy->SetTexture(groundTexture);
	enemy->SetScale(vec3(1.0f, 1.0f, 1.0f));
	cubeRigidBody->setUserPointer(enemy);


}
void CustomUpdate(btDynamicsWorld* dynamicsWorld, btScalar dt) { // Custom update of dynamicsWorld (additional to physics).
	if (!gameOver) {
		// Get enemy transform
		btTransform transformEnemy(enemy->rigidBody->getWorldTransform()); // WorldMatrix*PosMatrix

		// Set enemy position
		btVector3 velocity(-15.0f, 0, 0);
		//btVector3 velocity(0.0f, 0, 0);
		transformEnemy.setOrigin(transformEnemy.getOrigin() + velocity * dt);

		// Check if enemy is on offScreen
		if (transformEnemy.getOrigin().x() <= -18.0f) {
			transformEnemy.setOrigin(btVector3(18, 1, 0)); // back to the right of the viewport
			score++;
			//printf("Score: %i\n", score);
			label->SetText("Score: " + std::to_string(score));
		}
		enemy->rigidBody->setWorldTransform(transformEnemy);
		enemy->rigidBody->getMotionState()->setWorldTransform(transformEnemy);
	}
	// Check every collisions
	grounded = false;
	int numCollisions = dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int n = 0; n < numCollisions; n++) {
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(n);
		int numContacts = contactManifold->getNumContacts(); // Number of objects in contact.
		if (numContacts > 0) {
			
			const btCollisionObject* bodyA = contactManifold->getBody0();
			const btCollisionObject* bodyB = contactManifold->getBody1();
			MESH_RENDER* meshA = (MESH_RENDER*)bodyA->getUserPointer();
			MESH_RENDER* meshB = (MESH_RENDER*)bodyB->getUserPointer();
			
			if ((meshA->name == "hero" && meshB->name == "enemy") || 
			(meshA->name == "enemy" && meshB->name == "hero")) {
				//printf("Collision: %s with %s \n", meshA->name, meshB->name);
				if (meshB->name == "enemy") {
					btTransform transEnemy(meshB->rigidBody->getWorldTransform());
					transEnemy.setOrigin(btVector3(18, 1, 0)); // back to the right of the viewport
					meshB->rigidBody->setWorldTransform(transEnemy);
					meshB->rigidBody->getMotionState()->setWorldTransform(transEnemy);
				} else { // If meshA is enemy
					btTransform transEnemy(meshA->rigidBody->getWorldTransform());
					transEnemy.setOrigin(btVector3(18, 1, 0)); // back to the right of the viewport
					meshA->rigidBody->setWorldTransform(transEnemy);
					meshA->rigidBody->getMotionState()->setWorldTransform(transEnemy);
				}
				gameOver = true;
				score = 0;
				label->SetText("Score: " + std::to_string(score));
				//printf("Score: %i\n", score);
			}
			if ((meshA->name == "hero" && meshB->name == "ground") ||
			(meshA->name == "ground" && meshB->name	== "hero")) {
				grounded = true;
				//printf("Collision: %s with %s \n", meshA->name, meshB->name);
			}

		}

	}

}
void UpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (gameOver)
			gameOver = false;
		else {
			if (grounded == true) {
				grounded = false;
				uvSphere->rigidBody->applyImpulse(
					btVector3(0.0f, 150.0f, 0.0f), // impulse force: 100 in y.
					btVector3(0.0f, 0.0f, 0.0f) // position from the center of mass where the impulse is applied -> can be rotation
				);
			}
		}
	}
}
int main(int argc, char** argv) {
	glfwSetErrorCallback(&GlfwError);

	glfwInit();
	
	GLFWwindow* window = glfwCreateWindow(800, 600, " Hell world ", NULL, NULL);
	
	glfwMakeContextCurrent(window);
	// Capture keyboard events
	glfwSetKeyCallback(window, UpdateKeyboard);
	
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
	delete light;

	return 0;

}
void InitGame() {
	glEnable(GL_DEPTH_TEST); // GL_DEPTH_TEST: Depth texting -> only the pixels in the front are drawn

	// ******** Set shaders *********
	SHADER_LOADER shaderLoader;
	flatShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/FLAT_MODEL.vs", "Assets/Shaders/FLAT_MODEL.fs");
	textureShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/TEXTURE_MODEL.vs", "Assets/Shaders/TEXTURE_MODEL.fs");
	litTextureShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/LIT_TEXTURE_MODEL.vs", "Assets/Shaders/LIT_TEXTURE_MODEL.fs");
	textShaderProgram = shaderLoader.CreateProgram("Assets/Shaders/TEXT_MODEL.vs", "Assets/Shaders/TEXT_MODEL.fs");
	
	// ******** Set camera *********
	camera = new CAMERA(45.0f, 800, 600, 0.1f, 100.0f, vec3(0.0f, 4.0f, 20.0f)); // 800x600: size of window

	// ******** Light mesh without texture *********
	light = new LIGHT_RENDER(MESH_TYPE::UVsphere, camera);
	light->SetProgram(flatShaderProgram);
	light->SetPositon(vec3(0.0f, 10.0f, 0.0f));
	light->SetColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// ******** Texture loader *********
	TEXTURE_LOADER textureLoader;
	sphereTexture = textureLoader.GetTextureID("Assets/Textures/globe.jpg");
	groundTexture = textureLoader.GetTextureID("Assets/Textures/ground.jpg");

	// ******** Set score label *********
	label = new TEXT_RENDER("Score: 0", "Assets/Fonts/gooddog.ttf", 64, vec3(1.0f, 0.0f, 0.0f), textShaderProgram); // Text height: 64
	label->SetPosition(glm::vec2(320.0f, 500.0f));

	// ******** Load physics *********
	btBroadphaseInterface* broadPhaseCollision = new btDbvtBroadphase(); // broadphase: Using bounding boxes of the objects
	btDefaultCollisionConfiguration* defalultCollisionConf = new btDefaultCollisionConfiguration(); // Default memory.
	btCollisionDispatcher* dispatcherCollision = new btCollisionDispatcher(defalultCollisionConf); // details of the collision detection, such as which object collided with which other object.
	btSequentialImpulseConstraintSolver* constrains = new btSequentialImpulseConstraintSolver(); // can restrict the motion or rotation
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcherCollision, broadPhaseCollision, constrains, defalultCollisionConf);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
	dynamicsWorld->setInternalTickCallback(CustomUpdate);
	
	AddRigidBodMesh();
}
void RenderScene(GLclampf red = 0.0, GLclampf green = 0.0, GLclampf blue = 0.0, GLclampf alpha = 1.0) { // Clampled 32 bits float, clamped to the range [0, 1]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer and the depth buffer (if a pixel is behind another pixel, then that pixel will not be stored and show).
	
	glClearColor(red, green, blue, alpha); // Red.  The buffers need to be cleared in every frame.
	
	// Draw game objects
	light->Draw();
	uvSphere->Draw();
	ground->Draw();
	enemy->Draw();
	label->Draw();
}
