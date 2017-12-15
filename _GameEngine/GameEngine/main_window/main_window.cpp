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
	Shader emissionShader("Emission.vs", "Emission.fs");
	Shader negativeShader("Animated.vs", "Negatives.fs");

	GameObject Ground;
	Ground.setScale(2, 2, 2);
	AnimatedModel groundMesh(&Ground, "PlayGround.fbx", &staticShader);
	groundMesh.setTexture("PlayGroundBaked.bmp", "Diffuse");
	groundMesh.setTexture("GroundAO.bmp", "AO");

	GameObject Characters[3];
	SphereCollider characterColliders[3];

	GameObject team2Characters[3];
	SphereCollider team2characterColliders[3];

	//==============================Team 1===============================//
	Characters[0].setPosition(-16, 0, -5.5);
	Characters[0].setRotation(0, 70, 0);
	Characters[0].name = "Heavy weapon";
	Characters[0].tag = "team_1";
	AnimatedModel characterMesh(&Characters[0], "character01.fbx", &animatedShader);
	characterMesh.setTexture("CharacterHWColor.bmp", "Diffuse");
	characterMesh.setTexture("CharacterHWAO.bmp", "AO");
	characterColliders[0].Init(&Characters[0], 0.5f, glm::vec4(0, 1, 0, 1), false);
	characterColliders[0].setMinMax(-27, 27, -17, 17);


	//GameObject Character02;
	Characters[1].setPosition(-16, 0, 0);
	Characters[1].setRotation(0, 90, 0);
	Characters[1].name = "Shield";
	Characters[1].tag = "team_1";
	AnimatedModel character2Mesh(&Characters[1], "Character02.fbx", &animatedShader);
	character2Mesh.setTexture("CharacterShieldColor.bmp", "Diffuse");
	character2Mesh.setTexture("CharacterShieldAO.bmp", "AO");
	characterColliders[1].Init(&Characters[1], 0.75f, glm::vec4(0, 1, 0, 1), false, 5.0f);
	characterColliders[1].setMinMax(-27, 27, -17, 17);

	//GameObject Character03;
	Characters[2].setPosition(-16, 0, 5.5);
	Characters[2].setRotation(0, 120, 0);
	Characters[2].name = "Speed";
	Characters[2].tag = "team_1";
	AnimatedModel character3Mesh(&Characters[2], "Character03.fbx", &animatedShader);
	character3Mesh.setTexture("CharacterSColor.bmp", "Diffuse");
	character3Mesh.setTexture("CharacterSAO.bmp", "AO");
	characterColliders[2].Init(&Characters[2], 0.5f, glm::vec4(0, 1, 0, 1), false, 0.35f);
	characterColliders[2].setMinMax(-27, 27, -17, 17);
	//==============================Team 1===============================//


	//==============================Team 2===============================//
	team2Characters[0].setPosition(16, 0, -5.5);
	team2Characters[0].setRotation(0, -70, 0);
	team2Characters[0].name = "Heavy weapon";
	team2Characters[0].tag = "team_1";
	AnimatedModel t2characterMesh(&team2Characters[0], "character01.fbx", &negativeShader);
	t2characterMesh.setTexture("CharacterHWColor.bmp", "Diffuse");
	t2characterMesh.setTexture("CharacterHWAO.bmp", "AO");
	team2characterColliders[0].Init(&team2Characters[0], 0.5f, glm::vec4(0, 1, 0, 1), false);
	team2characterColliders[0].setMinMax(-27, 27, -17, 17);


	//GameObject Character02;
	team2Characters[1].setPosition(16, 0, 0);
	team2Characters[1].setRotation(0, -90, 0);
	team2Characters[1].name = "Shield";
	team2Characters[1].tag = "team_1";
	AnimatedModel t2character2Mesh(&team2Characters[1], "Character02.fbx", &negativeShader);
	t2character2Mesh.setTexture("CharacterShieldColor.bmp", "Diffuse");
	t2character2Mesh.setTexture("CharacterShieldAO.bmp", "AO");
	team2characterColliders[1].Init(&team2Characters[1], 0.75f, glm::vec4(0, 1, 0, 1), false, 5.0f);
	team2characterColliders[1].setMinMax(-27, 27, -17, 17);

	//GameObject Character03;
	team2Characters[2].setPosition(16, 0, 5.5);
	team2Characters[2].setRotation(0, -120, 0);
	team2Characters[2].name = "Speed";
	team2Characters[2].tag = "team_1";
	AnimatedModel t2character3Mesh(&team2Characters[2], "Character03.fbx", &negativeShader);
	t2character3Mesh.setTexture("CharacterSColor.bmp", "Diffuse");
	t2character3Mesh.setTexture("CharacterSAO.bmp", "AO");
	team2characterColliders[2].Init(&team2Characters[2], 0.5f, glm::vec4(0, 1, 0, 1), false, 0.35f);
	team2characterColliders[2].setMinMax(-27, 27, -17, 17);
	//==============================Team 2===============================//

	glm::vec3 data1StartLocation = glm::vec3(26, 0, -6);
	GameObject DataTeam1;
	DataTeam1.setPosition(data1StartLocation.x, data1StartLocation.y, data1StartLocation.z);
	DataTeam1.setRotation(0, 90, 0);
	DataTeam1.name = "data";
	DataTeam1.tag = "team_1";
	AnimatedModel dataMesh(&DataTeam1, "DataFile.fbx", &staticShader);
	dataMesh.setTexture("DataColor.bmp", "Diffuse");
	dataMesh.setTexture("DataAO.bmp", "AO");
	SphereCollider data01Collider(&DataTeam1, 3, glm::vec4(0, 1, 0, 1), true);

	GameObject GoalTeam1;
	GoalTeam1.setPosition(-26, 0, 6);
	GoalTeam1.setRotation(0, 90, 0);
	GoalTeam1.name = "goal";
	GoalTeam1.tag = "team_1";
	AnimatedModel goalMesh(&GoalTeam1, "goal.fbx", &staticShader);
	goalMesh.setTexture("github.bmp", "Diffuse");
	goalMesh.setTexture("whiteAO.bmp", "AO");
	SphereCollider goal01Collider(&GoalTeam1, 3, glm::vec4(0, 1, 0, 1), true);

	GameObject DataTeam2;
	DataTeam2.setPosition(-26, 0, -5);
	DataTeam2.setRotation(0, 90, 0);
	DataTeam2.name = "data";
	DataTeam2.tag = "team_2";
	AnimatedModel data2Mesh(&DataTeam2, "DataFile.fbx", &staticShader);
	data2Mesh.setTexture("DataColor.bmp", "Diffuse");
	data2Mesh.setTexture("DataAO.bmp", "AO");
	SphereCollider data02Collider(&DataTeam2, 3, glm::vec4(0, 1, 0, 1), true);

	GameObject GoalTeam2;
	GoalTeam2.setPosition(26, 0, 6);
	GoalTeam2.setRotation(0, 90, 0);
	GoalTeam2.name = "goal";
	GoalTeam2.tag = "team_2";
	AnimatedModel goal2Mesh(&GoalTeam2, "goal.fbx", &staticShader);
	goal2Mesh.setTexture("github.bmp", "Diffuse");
	goal2Mesh.setTexture("whiteAO.bmp", "AO");
	SphereCollider goal02Collider(&GoalTeam2, 3, glm::vec4(0, 1, 0, 1), true);

	GameObject skyBox;
	skyBox.setScale(500, 500, 500);
	skyBox.setRotation(90, 0, 0);
	skyBox.setPosition(0, -100, 0);
	AnimatedModel skyBoxMesh(&skyBox, "skybox.fbx", &staticShader);
	skyBoxMesh.setTexture("space big2ud.bmp", "Diffuse");
	skyBoxMesh.setTexture("whiteAO.bmp", "AO");

	GameObject shot;
	shot.setPosition(0, -1000, 0);
	shot.setScale(0.5f, 0.5f, 0.5f);
	AnimatedModel shotMesh(&shot, "shot.fbx", &emissionShader);
	shotMesh.setTexture("whiteAO.bmp", "Diffuse");
	shotMesh.setTexture("whiteAO.bmp", "AO");
	SphereCollider shotCollider(&shot, 0.5f, glm::vec4(0, 0, 0, 1), false);
	shotCollider.setMinMax(-100, 100, -100, 100);

	GameObject scoreScreen;
	scoreScreen.setPosition(0, 6.5f, 0);
	AnimatedModel scoreScreenMesh(&scoreScreen, "scoreScreen.fbx", &staticShader);
	scoreScreenMesh.setTexture("scoreScreenColor.bmp", "Diffuse");
	scoreScreenMesh.setTexture("whiteAO.bmp", "AO");


	logoUI.setTexture("LogoPresented.bmp");
	logoUI.position = glm::vec3(-0.75, 0.65, 0);
	logoUI.scale = glm::vec3(0.25, 0.25, 0);

	groundMesh.universalViewProj = &viewProj;
	dataMesh.universalViewProj = &viewProj;
	data2Mesh.universalViewProj = &viewProj;
	goalMesh.universalViewProj = &viewProj;
	goal2Mesh.universalViewProj = &viewProj;
	characterMesh.universalViewProj = &viewProj;
	character2Mesh.universalViewProj = &viewProj;
	character3Mesh.universalViewProj = &viewProj;
	t2characterMesh.universalViewProj = &viewProj;
	t2character2Mesh.universalViewProj = &viewProj;
	t2character3Mesh.universalViewProj = &viewProj;
	skyBoxMesh.universalViewProj = &viewProj;
	scoreScreenMesh.universalViewProj = &viewProj;
	shotMesh.universalViewProj = &viewProj;


	int team1score = 0;
	int selectedCharacter = 0;
	int holdingCharacter = -1;
	GameObject camera;
	camera.setPosition(-0.5f, 0.75f, -2);

	bool selectedPlayer = false;
	bool keyPressed = false;
	//Selecting player loop
	while (!selectedPlayer)
	{
		camera.setPosition(-5 + (selectedCharacter - 1) * 3, 0.5, 5 + (selectedCharacter - 1) * 3);
		camera.setRotation(0, -90, 0);

		viewProj = camera.getViewProjection();

		if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS && !keyPressed) {
			if (selectedCharacter > 0)
				selectedCharacter--;

			keyPressed = true;
		}
		if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS && !keyPressed) {
			if (selectedCharacter < 2)
				selectedCharacter++;

			keyPressed = true;
		}
		if (glfwGetKey(GLFW_KEY_ENTER) == GLFW_PRESS) {
			selectedPlayer = true;
		}

		if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(GLFW_KEY_RIGHT) == GLFW_RELEASE) {
			keyPressed = false;
		}

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
	}

	camera.setPosition(-0.5f, 0.75f, -2);
	camera.setRotation(0, 0, 0);
	camera.setParent(&Characters[selectedCharacter]);

	bool ctrlPressed = false;

	do {

		//Getting Inputs------------------------------------//
		if (glfwGetKey('W') == GLFW_PRESS || glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
			CollisionState cs = characterColliders[selectedCharacter].ApplyForce(Characters[selectedCharacter].getForwardVector() / 10.0f);
			if (cs == COLLIDED) {
				std::cout << "Collided with " << characterColliders[selectedCharacter].m_CollidedObjectName << std::endl;
			}
			else if (cs == TRIGGERED) {
				std::cout << "Triggered with " << characterColliders[selectedCharacter].m_CollidedObjectName << " " << characterColliders[selectedCharacter].m_CollidedObjectTag << std::endl;

				if (characterColliders[selectedCharacter].m_CollidedObjectTag == Characters[selectedCharacter].tag) {
					if (characterColliders[selectedCharacter].m_CollidedObjectName == "data") {
						holdingCharacter = selectedCharacter;
						data01Collider.m_active = false;
					}
					if (characterColliders[selectedCharacter].m_CollidedObjectName == "goal" && selectedCharacter == holdingCharacter) {
						holdingCharacter = -1;
						DataTeam1.setPosition(data1StartLocation.x, data1StartLocation.y, data1StartLocation.z);
						DataTeam1.setScale(1, 1, 1);
						team1score++;
						data01Collider.m_active = true;
					}
				}

			}
		}
		if (glfwGetKey('S') == GLFW_PRESS || glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
			CollisionState cs = characterColliders[selectedCharacter].ApplyForce(Characters[selectedCharacter].getForwardVector() / -100.0f);
			if (cs == COLLIDED) {
				std::cout << "Collided with " << characterColliders[selectedCharacter].m_CollidedObjectName << std::endl;
			}
			else if (cs == TRIGGERED) {
				std::cout << "Triggered with " << characterColliders[selectedCharacter].m_CollidedObjectName << std::endl;
			}
		}
		if (glfwGetKey('A') == GLFW_PRESS || glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
			Characters[selectedCharacter].RotateAroundY(1);
		}
		if (glfwGetKey('D') == GLFW_PRESS || glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
			Characters[selectedCharacter].RotateAroundY(-1);
		}

		if (glfwGetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
			if (Characters[selectedCharacter].name == "Heavy weapon") {
				glm::vec3 pos = Characters[selectedCharacter].getPosition() + 2.0f * Characters[selectedCharacter].getForwardVector();
				glm::vec3 rot = Characters[selectedCharacter].getRotation();

				shot.setPosition(pos.x, pos.y + 1, pos.z - 0.1f);
				shot.setRotation(rot.x, rot.y, rot.z);
			}
		}

		if (glfwGetKey(GLFW_KEY_LCTRL) == GLFW_PRESS && !ctrlPressed) {
			selectedCharacter++;
			selectedCharacter %= 3;
			ctrlPressed = true;
			camera.setParent(&Characters[selectedCharacter]);
		}
		if (glfwGetKey(GLFW_KEY_LCTRL) == GLFW_RELEASE) {
			ctrlPressed = false;
		}

		//-----------------------------------------------------------------------------------------------//

		if (holdingCharacter != -1) {
			DataTeam1.setPosition(Characters[holdingCharacter].getPosition().x, Characters[holdingCharacter].getPosition().y + 2.0f, Characters[holdingCharacter].getPosition().z);
			DataTeam1.setScale(0.25f, 0.25f, 0.25f);
		}
		else {
			DataTeam1.setPosition(DataTeam1.getPosition().x, 0, DataTeam1.getPosition().z);
			DataTeam1.setScale(1, 1, 1);
		}

		shotCollider.ApplyForce(shot.getForwardVector());

		//std::cout << "team 01 score = " << team1score << std::endl;

		DataTeam1.setRotation(0, glfwGetTime() * 50, 0);
		DataTeam2.setRotation(0, -glfwGetTime() * 50, 0);
		scoreScreen.setRotation(0, -glfwGetTime() * 50, 0);

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

