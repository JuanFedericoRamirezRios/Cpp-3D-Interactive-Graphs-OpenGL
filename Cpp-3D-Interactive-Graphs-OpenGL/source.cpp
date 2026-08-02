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
#include "POINT_LIGHT.hpp"
#include "GAME_OBJECT.hpp"
#include "TEXTURE_LOADER.hpp"
#include "TEXT_RENDER.hpp"
#include "OGL_UTILITIES.hpp"

using namespace glm;

CAMERA* camera;
POINT_LIGHT* pointLight;
GAME_OBJECT* uvSphere;
GAME_OBJECT* ground;
GAME_OBJECT* enemy;
TEXT_RENDER* label;

GAME_OBJECT* blueBox;
GAME_OBJECT* textureSph;
GAME_OBJECT* textureLitSph;

btDiscreteDynamicsWorld* dynamicsWorld; // Track of all the physics.

GLuint flatShaderProgram, textureShaderProgram, litTextureShaderProgram, textShaderProgram;

GLuint sphereTexture, groundTexture;

bool grounded = false;
bool gameOver = true;
int score = 0;

void UpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void InitGame();
void Script(btDynamicsWorld* dynamicsWorld, btScalar dt);
void AddGameObjects();
void RenderScene(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

static void GlfwError(int id, const char* description) {
	std::cerr << "GLFW Error: " << description << std::endl;
}

int main(int argc, char** argv) {
	GLFWwindow* window = OGL_FEDE::InitWindow(800, 600, "Game example");

	// Capture keyboard events
	glfwSetKeyCallback(window, UpdateKeyboard);

	InitGame();
	AddGameObjects();


	auto t0 = std::chrono::high_resolution_clock::now();
		
	while (!glfwWindowShouldClose(window)) {
		auto t = std::chrono::high_resolution_clock::now();
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
	delete pointLight;

	return 0;

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

	// ******** Point Light *********
	pointLight = new POINT_LIGHT(vec3(0.0f, 10.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// ******** Texture loader *********
	TEXTURE_LOADER textureLoader;
	sphereTexture = textureLoader.GetTextureID("Assets/Textures/globe.jpg");
	groundTexture = textureLoader.GetTextureID("Assets/Textures/ground.jpg");

	// ******** Set score label *********
	label = new TEXT_RENDER("Score: 0", "Assets/Fonts/gooddog.ttf", 64, vec3(1.0f, 0.0f, 0.0f), textShaderProgram); // Text height: 64
	label->SetPosition(glm::vec2(320.0f, 500.0f));

	// ******** Load physics *********
	dynamicsWorld = OGL_FEDE::PhysicsWorld(btVector3(0.0f, -9.8f, 0.0f));

	// ******** Load Script *********
	dynamicsWorld->setInternalTickCallback(Script);
}
void Script(btDynamicsWorld* dynamicsWorld, btScalar dt) { // Custom update of dynamicsWorld (additional to physics).
	if (!gameOver) {
		btVector3 velocity(-15.0f, 0, 0);

		//btTransform transEnemy = enemy->rigidBody->getWorldTransform(); // WorldMatrix*PosMatrix
		btTransform transEnemy(enemy->rigidBody->getWorldTransform()); // WorldMatrix*PosMatrix
		transEnemy.setOrigin(transEnemy.getOrigin() + velocity * dt);

		// Check if enemy is on offScreen
		if (transEnemy.getOrigin().x() <= -18.0f) {
			transEnemy.setOrigin(btVector3(18, 1, 0)); // back to the right of the viewport

			score++;
			label->SetText("Score: " + std::to_string(score));
		}
		enemy->rigidBody->setWorldTransform(transEnemy);
		enemy->rigidBody->getMotionState()->setWorldTransform(transEnemy);
	}
	// Check every collisions
	grounded = false;

	std::string namesCol[2] = { "","" };
	if (Check2RBcol(dynamicsWorld, namesCol)) {
		if ((namesCol[0] == "hero" && namesCol[1] == "enemy") ||
			(namesCol[0] == "enemy" && namesCol[1] == "hero")) {
			btTransform transEnemy(enemy->rigidBody->getWorldTransform());
			transEnemy.setOrigin(btVector3(18, 1, 0)); // back to the right of the viewport
			enemy->rigidBody->setWorldTransform(transEnemy);
			enemy->rigidBody->getMotionState()->setWorldTransform(transEnemy);
			gameOver = true;
			score = 0;
			label->SetText("Score: " + std::to_string(score));
		}
		if ((namesCol[0] == "hero" && namesCol[1] == "ground") || (namesCol[0] == "ground" && namesCol[1] == "hero")) {
			grounded = true;
		}
	}

}
void AddGameObjects() {
	// ******** Sphere *********
	uvSphere = new GAME_OBJECT("hero", vec3(-2.0f, 0.5f, 0), vec3(1.0f));

	uvSphere->SetVertex(MESH_TYPE::UVsphere);
	//uvSphere->SetDefaultColor(flatShaderProgram);
	//uvSphere->SetColor(flatShaderProgram, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	uvSphere->SetTexture(textureShaderProgram, sphereTexture);
	//uvSphere->SetTextureLit(litTextureShaderProgram, sphereTexture, pointLight, 0.1f, 0.5f);

	uvSphere->SetSphereRB(1.0f, 13.0f);
	uvSphere->rigidBody->setActivationState(DISABLE_DEACTIVATION); // We need to control the jump.
	dynamicsWorld->addRigidBody(uvSphere->rigidBody);

	// ******** Ground *********
	ground = new GAME_OBJECT("ground", vec3(0, -1.0f, 0), vec3(4.0f, 0.5f, 4.0f));

	ground->SetVertex(MESH_TYPE::Cube);
	ground->SetTextureLit(litTextureShaderProgram, groundTexture, pointLight, 0.1f, 0.5f);

	ground->SetBoxRB(0.0f);
	ground->rigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT); // will be like a brick wall and won't move and get affected by forces from other rigid	bodies, but other bodies will be affected by it.
	dynamicsWorld->addRigidBody(ground->rigidBody);

	// ******** Enemy *********
	enemy = new GAME_OBJECT("enemy", vec3(18.0f, 1.0f, 0), vec3(1.0f, 1.0f, 1.0f));

	enemy->SetVertex(MESH_TYPE::Cube);
	enemy->SetColor(flatShaderProgram, vec4(1.0f, 0.0f, 0.0f, 1.0f));

	enemy->SetBoxRB(0.0f);
	//enemy->rigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT); // exert force on other	objects.
	enemy->rigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE); // CF_NO_CONTACT_RESPONSE: No check if there was an overlap between the enemy rigid body and another body. CF_KINEMATIC_OBJECT: The objects respond to collision.
	dynamicsWorld->addRigidBody(enemy->rigidBody);

	// ********* Decoration objetcs (no rigidBody -> no physics) ******
	blueBox = new GAME_OBJECT("", vec3(-2.0f, 7.0f, 0.0f), vec3(1.0f));
	blueBox->SetVertex(MESH_TYPE::Cube);
	blueBox->SetColor(flatShaderProgram, vec4(0.0f, 0.0f, 1.0f, 1.0f));

	textureSph = new GAME_OBJECT("", vec3(2.0f, 7.0f, -5.0f), vec3(2.0f));
	textureSph->SetVertex(MESH_TYPE::UVsphere);
	textureSph->SetTexture(litTextureShaderProgram, groundTexture);

	textureLitSph = new GAME_OBJECT("", vec3(9.0f, 1.0f, 0), vec3(2.0f));
	textureLitSph->SetVertex(MESH_TYPE::UVsphere);
	textureLitSph->SetTextureLit(litTextureShaderProgram, groundTexture, pointLight, 0.1f, 0.5f);

}
void RenderScene(GLclampf red = 0.0, GLclampf green = 0.0, GLclampf blue = 0.0, GLclampf alpha = 1.0) { // Clampled 32 bits float, clamped to the range [0, 1]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer and the depth buffer (if a pixel is behind another pixel, then that pixel will not be stored and show).
	
	glClearColor(red, green, blue, alpha); // Red.  The buffers need to be cleared in every frame.
	
	// Draw game objects
	uvSphere->Draw(camera);
	ground->Draw(camera);
	enemy->Draw(camera);
	label->Draw();

	blueBox->Draw(camera);
	textureSph->Draw(camera);
	textureLitSph->Draw(camera);
}
