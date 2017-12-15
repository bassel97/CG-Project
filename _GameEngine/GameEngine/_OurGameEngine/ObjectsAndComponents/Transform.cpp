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

	Parent = NULL;
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
	glm::mat4 transform;

	if (Parent == NULL)
		transform = glm::translate(glm::mat4(1.0f), Position);
	else
		transform = glm::translate(Parent->getTransformMatrix(), Position);

	transform = glm::rotate(transform, Rotaion.x, glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, Rotaion.y, glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, Rotaion.z, glm::vec3(0, 0, 1));

	transform = glm::scale(transform, Scale);

	return transform;
}

glm::mat4 Transform::getViewProjection()
{
	glm::vec4 forward;
	if (Parent == NULL)
		forward = glm::rotateY(glm::vec4(0, 0, 1, 1), Rotaion.y);
	else
		forward = glm::rotateY(glm::vec4(0, 0, 1, 1), Parent->Rotaion.y);

	glm::vec3 forVec3 = glm::vec3(forward.x, forward.y, forward.z);

	glm::vec4 positionFromParent = getTransformMatrix() * glm::vec4(Position.x, Position.y, Position.z, 1.0);

	glm::vec3 posVec3 = glm::vec3(positionFromParent.x, positionFromParent.y, positionFromParent.z);

	glm::mat4 viewProj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 600.0f) * glm::lookAt(
		posVec3,
		posVec3 + forVec3,
		glm::vec3(0, 1, 0)
	);

	return viewProj;
}

glm::mat4 Transform::getRotationMatrix()
{
	glm::mat4 transform;

	transform = glm::rotate(transform, Rotaion.x, glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, Rotaion.y, glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, Rotaion.z, glm::vec3(0, 0, 1));

	return transform;
}

glm::vec3 Transform::getPosition()
{
	return Position;
}

glm::vec3 Transform::getRotation()
{
	return Rotaion;
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

void Transform::Translate(glm::vec3 displacment)
{
	Position += displacment;
}

void Transform::RotateAroundY(float value)
{
	Rotaion.y += value;
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