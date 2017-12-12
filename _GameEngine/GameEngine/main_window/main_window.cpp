// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;


//Our Engine Includes
#include "main_game\MainGame.hpp"
#include "_OurGameEngine\Component.hpp"
#include "_OurGameEngine\ObjectsAndComponents\Transform.hpp"
#include "_OurGameEngine\ObjectsAndComponents\MeshRenderer.hpp"
#include "_OurGameEngine\ObjectsAndComponents\GUI.hpp"
#include "_OurGameEngine\ObjectsAndComponents\AnimatedModel.hpp"

#include"common\controls.hpp"
#include"_OurGameEngine\ObjectsAndComponents\SpherePhysics.hpp" 

#include "btBulletDynamicsCommon.h"

//Static variabled declared here because component has no .cpp file
std::vector<Component*> Component::allComponents;
std::vector<SphereCollider*> SphereCollider::m_PhysicsComponents;

#include<assimp\Importer.hpp>

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	//The following line specifies that we will use 4x anti aliasing. Anti-aliasing is the technique of minimizing the distortion artifacts known as aliasing when representing a high-resolution image at a lower resolution.
	//FSAA stands for full-scene anti-aliasing which makes the image appear softer, and should also appear more realistic.   
	//FSAA means that each full frame is rendered at double (2x) or quadruple (4x) the display resolution, and then down-sampled to match the display resolution.
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);


	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	//glfwOpenWindow( int width, int height, int redbits,int greenbits, int bluebits, int alphabits, int depthbits,int stencilbits, int mode ) 
	//redbits,greenbits,bluebits: number of bits to use for each color.(0 means default color depth) 
	//mode: specifies whether we will open a GLFW_FullScreen or a GLFW_WINDOW
	//if (!glfwOpenWindow(1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
	if (!glfwOpenWindow(1920, 1080, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle("Project Delivery Game Window");

	// Ensure we can capture the escape key being pressed below
	glfwEnable(GLFW_STICKY_KEYS);

	// Dark blue background
	glClearColor(0.15f, 0.15f, 0.2f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	////----------------------------Bullet Physics----------------------------------------------------------------//
	//// Initialize Bullet. This strictly follows http://bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World, 
	//// even though we won't use most of this stuff.

	//// Build the broadphase
	//btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	//// Set up the collision configuration and dispatcher
	//btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	//btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//// The actual physics solver
	//btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	//// The world.
	//btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	//dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	////----------------------------Bullet Physics----------------------------------------------------------------//


	//universal view-projection matrix
	glm::mat4 viewProj;



	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader guiSh("GUI.vs", "GUI.fs");
	GameObject Logo;
	GUI logoUI(&Logo, &guiSh, "Loading.bmp");
	logoUI.scale = glm::vec3(0.5, 0.5, 0.5);
	logoUI.Update();
	// Swap buffers
	glfwSwapBuffers();

	Shader staticShader("Static.vs", "Static.fs");
	Shader animatedShader("Animated.vs", "Animated.fs");

	GameObject Ground;
	Ground.setScale(2, 2, 2);
	AnimatedModel groundMesh(&Ground, "PlayGround.fbx", &staticShader);
	groundMesh.setTexture("PlayGroundBaked.bmp", "Diffuse");
	groundMesh.setTexture("GroundAO.bmp", "AO");

	GameObject Character01;
	Character01.setPosition(16, 0, 0);
	Character01.setRotation(0, -90, 0);
	Character01.name = "Heavy weapon";
	AnimatedModel characterMesh(&Character01, "character01.fbx", &animatedShader);
	characterMesh.setTexture("CharacterHWColor.bmp", "Diffuse");
	characterMesh.setTexture("CharacterHWAO.bmp", "AO");
	SphereCollider character01Collider(&Character01, 0.5f, glm::vec4(0, 1, 0, 1), false);
	character01Collider.setMinMax(-27, 27, -17, 17);


	GameObject Character02;
	Character02.setPosition(-16, 0, 0);
	Character02.setRotation(0, 70, 0);
	Character02.name = "Shield";
	AnimatedModel character2Mesh(&Character02, "Character02.fbx", &animatedShader);
	character2Mesh.setTexture("CharacterShieldColor.bmp", "Diffuse");
	character2Mesh.setTexture("CharacterShieldAO.bmp", "AO");
	SphereCollider character02Collider(&Character02, 0.5f, glm::vec4(0, 1, 0, 1), false);

	/*GameObject Character03;
	Character03.setPosition(-16, 0, 5.5);
	Character03.setRotation(0, 120, 0);
	AnimatedModel character3Mesh(&Character03, "Character03.fbx", &animatedShader);
	character3Mesh.setTexture("CharacterSColor.bmp", "Diffuse");
	character3Mesh.setTexture("CharacterSAO.bmp", "AO");

	GameObject Character04;
	Character04.setPosition(-16, 5, 0);
	Character04.setRotation(0, 90, 0);
	AnimatedModel character4Mesh(&Character04, "Character04.fbx", &animatedShader);
	character4Mesh.setTexture("CharceterXColor.bmp", "Diffuse");
	character4Mesh.setTexture("CharceterXAO.bmp", "AO");*/

	GameObject DataTeam1;
	DataTeam1.setPosition(26, 0, 0);
	DataTeam1.setRotation(0, 90, 0);
	DataTeam1.name = "data 1";
	AnimatedModel dataMesh(&DataTeam1, "DataFile.fbx", &staticShader);
	dataMesh.setTexture("DataColor.bmp", "Diffuse");
	dataMesh.setTexture("DataAO.bmp", "AO");
	SphereCollider data01Collider(&DataTeam1, 3, glm::vec4(0, 1, 0, 1), true);

	GameObject DataTeam2;
	DataTeam2.setPosition(-26, 0, 0);
	DataTeam2.setRotation(0, 90, 0);
	DataTeam2.name = "data 2";
	AnimatedModel data2Mesh(&DataTeam2, "DataFile.fbx", &staticShader);
	data2Mesh.setTexture("DataColor.bmp", "Diffuse");
	data2Mesh.setTexture("DataAO.bmp", "AO");
	SphereCollider data02Collider(&DataTeam2, 3, glm::vec4(0, 1, 0, 1), true);

	GameObject skyBox;
	skyBox.setScale(500, 500, 500);
	skyBox.setRotation(90, 0, 0);
	skyBox.setPosition(0, -100, 0);
	AnimatedModel skyBoxMesh(&skyBox, "skybox.fbx", &staticShader);
	skyBoxMesh.setTexture("space big2ud.bmp", "Diffuse");
	skyBoxMesh.setTexture("whiteAO.bmp", "AO");

	GameObject scoreScreen;
	scoreScreen.setPosition(0, 10, 0);
	AnimatedModel scoreScreenMesh(&scoreScreen, "scoreScreen.fbx", &staticShader);
	scoreScreenMesh.setTexture("scoreScreenColor.bmp", "Diffuse");
	scoreScreenMesh.setTexture("whiteAO.bmp", "AO");

	GameObject camera;
	camera.setParent(&Character01);
	camera.setPosition(-0.5f, 0.75f, -2);


	logoUI.setTexture("LogoPresented.bmp");
	logoUI.position = glm::vec3(-0.75, 0.65, 0);
	logoUI.scale = glm::vec3(0.25, 0.25, 0);


	groundMesh.universalViewProj = &viewProj;
	dataMesh.universalViewProj = &viewProj;
	data2Mesh.universalViewProj = &viewProj;
	characterMesh.universalViewProj = &viewProj;
	character2Mesh.universalViewProj = &viewProj;
	/*character3Mesh.universalViewProj = &viewProj;
	character4Mesh.universalViewProj = &viewProj;*/
	skyBoxMesh.universalViewProj = &viewProj;
	scoreScreenMesh.universalViewProj = &viewProj;


	do {

		//Getting Inputs
		if (glfwGetKey('W') == GLFW_PRESS || glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
			CollisionState cs = character01Collider.ApplyForce(Character01.getForwardVector() / 10.0f);
			if (cs == COLLIDED) {
				std::cout << "Collided with " << character01Collider.m_CollidedObjectName << std::endl;
			}
			else if (cs == TRIGGERED) {
				std::cout << "Triggered with " << character01Collider.m_CollidedObjectName << std::endl;
			}
		}
		if (glfwGetKey('S') == GLFW_PRESS || glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
			CollisionState cs = character01Collider.ApplyForce(Character01.getForwardVector() / -10.0f);
			if (cs == COLLIDED) {
				std::cout << "Collided with " << character01Collider.m_CollidedObjectName << std::endl;
			}
			else if (cs == TRIGGERED) {
				std::cout << "Triggered with " << character01Collider.m_CollidedObjectName << std::endl;
			}
		}
		if (glfwGetKey('A') == GLFW_PRESS || glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
			Character01.RotateAroundY(1);
		}
		if (glfwGetKey('D') == GLFW_PRESS || glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
			Character01.RotateAroundY(-1);
		}

		DataTeam1.setRotation(0, glfwGetTime() * 50, 0);
		DataTeam2.setRotation(0, -glfwGetTime() * 50, 0);
		scoreScreen.setRotation(0, -glfwGetTime() * 50, 0);

		//testAssimp.setRotation(0, glfwGetTime() * 90, 0);

		//go2.setRotation(45, 90, 0);
		//glm::vec3 go2Up = go2.getUpVector();
		//glm::vec3 go2F = go2.getForwardVector();

		//glm::vec3 pos = glm::vec3(-sin(glfwGetTime() / 2) * 6, 5.5, sin(glfwGetTime() / 2 + 1) * 5 - 4.5);
		/*glm::vec3 pos = glm::vec3(0, 7, 0);
		glm::vec3 forward = glm::rotateY(glm::vec3(-1, -0.5, 0), (float)glfwGetTime() * 50);
		viewProj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f) * glm::lookAt(
			pos,
			pos + forward,
			glm::vec3(0, 1, 0)
		);*/

		//cubeMesh.vp = groundMesh.vp;
		//viewProj = groundMesh.vp;

		//computeMatricesFromInputs();
		//viewProj = getProjectionMatrix() * getViewMatrix();
		viewProj = camera.getViewProjection();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Calling Transform Function in Every game component First to apply new position,rotation,scale
		for (int i = 0; i < Transform::allComponents.size(); i++)
		{
			Transform::allComponents[i]->Update();
		}

		//Calling Update Function in Every game component
		for (int i = 0; i < Component::allComponents.size(); i++)
		{
			Component::allComponents[i]->Update();
		}

		// Swap buffers
		glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
		glfwGetWindowParam(GLFW_OPENED));

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

