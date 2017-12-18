#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "Transform.hpp"
#include "_OurGameEngine\Component.hpp"

#include <vector>

class GameObject : public Transform
{
public:
	GameObject();
	~GameObject();

	//Keeps track to all attached components to this Object
	std::vector<Component*> attachedComponents;

	void Update();

	void Start();

	void addComponent(Component* addedComponent);

	float freeze;

private:

};

#endif