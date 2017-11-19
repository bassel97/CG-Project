#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

std::vector<Transform*> Transform::allComponents;

//Constructor
Transform::Transform() {
	//Add every Transform to the all the component list
	Transform::allComponents.push_back(this);


	Position = glm::vec3(0, 0, 0);

	Scale = glm::vec3(1, 1, 1);

	Rotaion = glm::vec3(0, 0, 0);
}

Transform::~Transform()
{
}

void Transform::Start()
{
}

void Transform::Update()
{

}

glm::mat4 Transform::getTransformMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position);


	transform = glm::rotate(transform, Rotaion.x, glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, Rotaion.y, glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, Rotaion.z, glm::vec3(0, 0, 1));

	transform = glm::scale(transform, Scale);

	return transform;
}

void Transform::setPosition(float x, float y, float z)
{
	Position.x = x;
	Position.y = y;
	Position.z = z;
}

void Transform::setScale(float x, float y, float z)
{
	Scale.x = x;
	Scale.y = y;
	Scale.z = z;
}

void Transform::setRotation(float x, float y, float z)
{
	Rotaion.x = x;
	Rotaion.y = y;
	Rotaion.z = z;
}

glm::vec3 Transform::getUpVector()
{
	glm::vec3 upV = glm::vec3(0, 1, 0);

	upV = glm::rotate(upV, Rotaion.x, glm::vec3(1, 0, 0));
	upV = glm::rotate(upV, Rotaion.y, glm::vec3(0, 1, 0));
	upV = glm::rotate(upV, Rotaion.z, glm::vec3(0, 0, 1));

	return upV;
}

glm::vec3 Transform::getForwardVector()
{
	glm::vec3 upV = glm::vec3(0, 0, 1);

	upV = glm::rotate(upV, Rotaion.x, glm::vec3(1, 0, 0));
	upV = glm::rotate(upV, Rotaion.y, glm::vec3(0, 1, 0));
	upV = glm::rotate(upV, Rotaion.z, glm::vec3(0, 0, 1));

	return upV;
}