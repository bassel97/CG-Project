#ifndef TRANSFORM
#define TRANSFORM

#include "_OurGameEngine\EnginePrime.hpp"

#include <glm\glm.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <vector>

class Transform :public EnginePrime
{
public:
	Transform();
	~Transform();

	//if the position is defined by a parent
	Transform* Parent;


	//all Transforms found in game
	static std::vector< Transform* > allComponents;


	void Start();

	void Update();

	//Set a new parent
	void setParent(Transform* Parent) {
		this->Parent = Parent;
	}

	//Clears Parent refrence not the object
	void clearParent() {
		this->Parent = NULL;
	}

	glm::mat4 getTransformMatrix();
	glm::mat4 getViewProjection();
	glm::mat4 getRotationMatrix();

	glm::vec3 getPosition();
	glm::vec3 getRotation();

	void setPosition(float x, float y, float z);

	void setScale(float x, float y, float z);

	void setRotation(float x, float y, float z);

	void Translate(glm::vec3 displacment);
	void RotateAroundY(float value);

	glm::vec3 getUpVector();
	glm::vec3 getForwardVector();

	std::string name;
	std::string tag;

private:

	//Positions in local space relative to the parent
	float xPosition, yPosition, zPosition;

	//Scale in local space relative to the parent
	float xScale, yScale, zScale;

	//Rotation in local space relative to the parent
	float xRotation, yRotation, zRotation;

	glm::vec3 Position;

	glm::vec3 Scale;

	glm::vec3 Rotaion;

	//Y-axis
	glm::vec3 upVector;
	//Z-axis
	glm::vec3 forwardVector;
	//X-axis
	glm::vec3 rightVector;


};

#endif