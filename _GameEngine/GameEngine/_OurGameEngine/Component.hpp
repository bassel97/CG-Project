#ifndef COMPONENT
#define COMPONENT

#include <_OurGameEngine\EnginePrime.hpp>

#include <vector>

//Forward declaration of gameObject Class
class GameObject;

//Abstract class contains certain behaviours
class Component : public EnginePrime
{
public:

	//all components found in game
	static std::vector<Component*> allComponents;


	//Add every component to the all the component list
	Component() {
		Component::allComponents.push_back(this);
	}

	//gets called once before game starts
	virtual void Start() = 0;

	//gets called every program iteration "Frame"
	virtual void Update() = 0;

protected:
	//GameObject Container for this Component
	GameObject* containingObject;

};

#endif