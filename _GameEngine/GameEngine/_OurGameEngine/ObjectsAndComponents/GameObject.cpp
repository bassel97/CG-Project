#include "GameObject.hpp"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
}

void GameObject::Start()
{
}

void GameObject::addComponent(Component * addedComponent)
{
	attachedComponents.push_back(addedComponent);
}

