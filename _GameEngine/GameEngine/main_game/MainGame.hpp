#ifndef MAIN_GAME
#define MAIN_GAME

#include "_OurGameEngine\ObjectsAndComponents\GameObject.hpp"
#include "_OurGameEngine\ObjectsAndComponents\MeshRenderer.hpp"


class MainGame
{
public:
	MainGame()
	{
	}

	~MainGame()
	{
	}

	static void gameCode() {

		GameObject *go = new GameObject;
		MeshRenderer *m = new MeshRenderer;
		m->Initialize(go, NULL, "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
		go->addComponent(m);

	}

private:

};

#endif