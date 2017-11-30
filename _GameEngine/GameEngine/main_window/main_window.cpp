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

//Static variabled declared here because component has no .cpp file
std::vector<Component*> Component::allComponents;

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

	glfwSetWindowTitle("Game Window");

	// Ensure we can capture the escape key being pressed below
	glfwEnable(GLFW_STICKY_KEYS);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	//universal view-projection matrix
	glm::mat4 viewProj;

	//Instantiate GameObject
	//GameObject go2;
	//MeshRenderer m2(&go2, "poinsr.obj", "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	//go2.setPosition(0, 2, 0);
	//go2.setRotation(-45, 0, 0);
	//go2.setScale(0.02, 0.02, .02);
	//go2.addComponent(&m2);

	/*GameObject cube;
	MeshRenderer cubeMesh(&cube, "cube.obj", "SimpleVertexShader.vertexshader", "SimpleFragmentShaderColor.fragmentshader");
	cubeMesh.hasMVP = true;
	cube.setPosition(0, 1, 0);*/

	/*GameObject ground;
	MeshRenderer groundMesh(&ground, "ground.obj", "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	ground.addComponent(&groundMesh);
	groundMesh.hasMVP = true;
	groundMesh.setTexture("playgroundbakedhigh.bmp");*/

	//GameObject testAssimp;
	////testAssimp.setPosition(0, 0, 3);
	//testAssimp.setScale(0.5f, 0.5f, 0.5f);
	////am.shader = &shader1;
	//AnimatedModel am(&testAssimp, "assimpTestAnim2.fbx", &shader1);
	//am.universalViewProj = &viewProj;

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

	GameObject DataTeam1;
	DataTeam1.setPosition(26, 0, 0);
	DataTeam1.setRotation(0, 90, 0);
	AnimatedModel dataMesh(&DataTeam1, "DataFile.fbx", &staticShader);
	dataMesh.setTexture("DataColor.bmp", "Diffuse");
	dataMesh.setTexture("DataAO.bmp", "AO");

	GameObject DataTeam2;
	DataTeam2.setPosition(-26, 0, 0);
	DataTeam2.setRotation(0, 90, 0);
	AnimatedModel data2Mesh(&DataTeam2, "DataFile.fbx", &staticShader);
	data2Mesh.setTexture("DataColor.bmp", "Diffuse");
	data2Mesh.setTexture("DataAO.bmp", "AO");

	GameObject Character01;
	Character01.setPosition(-16, 0, 0);
	Character01.setRotation(0, 90, 0);
	AnimatedModel characterMesh(&Character01, "Character01.fbx", &animatedShader);
	characterMesh.setTexture("CharacterHWColor.bmp", "Diffuse");
	characterMesh.setTexture("DataAO.bmp", "AO");

	GameObject Character02;
	Character02.setPosition(-16, 0, -5.5);
	Character02.setRotation(0, 70, 0);
	AnimatedModel character2Mesh(&Character02, "Character02.fbx", &animatedShader);
	character2Mesh.setTexture("CharacterShieldColor.bmp", "Diffuse");
	character2Mesh.setTexture("DataAO.bmp", "AO");

	logoUI.setTexture("LogoPresented.bmp");
	logoUI.position = glm::vec3(-0.5, -0.5, 0);

	do {

		DataTeam1.setRotation(0, glfwGetTime() * 50, 0);
		DataTeam2.setRotation(0, -glfwGetTime() * 50, 0);

		//testAssimp.setRotation(0, glfwGetTime() * 90, 0);

		//go2.setRotation(45, 90, 0);
		//glm::vec3 go2Up = go2.getUpVector();
		//glm::vec3 go2F = go2.getForwardVector();

		//glm::vec3 pos = glm::vec3(-sin(glfwGetTime() / 2) * 6, 5.5, sin(glfwGetTime() / 2 + 1) * 5 - 4.5);
		glm::vec3 pos = glm::vec3(0, 7, 0);
		glm::vec3 forward = glm::rotateY(glm::vec3(-1, -0.5, 0), (float)glfwGetTime() * 15);
		viewProj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f) * glm::lookAt(
			pos,
			pos + forward,
			glm::vec3(0, 1, 0)
		);

		//cubeMesh.vp = groundMesh.vp;
		//viewProj = groundMesh.vp;

		/*computeMatricesFromInputs();
		viewProj = getProjectionMatrix() * getViewMatrix();*/

		groundMesh.universalViewProj = &viewProj;
		dataMesh.universalViewProj = &viewProj;
		data2Mesh.universalViewProj = &viewProj;
		characterMesh.universalViewProj = &viewProj;
		character2Mesh.universalViewProj = &viewProj;



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

