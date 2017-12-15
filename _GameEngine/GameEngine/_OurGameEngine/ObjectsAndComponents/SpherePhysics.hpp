#ifndef SPHERE_PHYSICS
#define SPHERE_PHYSICS

#include <GL/glew.h>
#include <GL\glfw.h>

#include <vector>

#include "_OurGameEngine\Component.hpp"
#include "_OurGameEngine\ObjectsAndComponents\GameObject.hpp"
#include "_OurGameEngine\ObjectsAndComponents\shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "common\objloader.hpp"
#include "common\texture.hpp"


enum CollisionState {
	COLLIDED,
	NOT_COLLIDED,
	TRIGGERED
};

class SphereCollider : public Component
{
public:

	SphereCollider(GameObject * containingObject, float radius, glm::vec4 center, bool trigger = false, float weight = 1);

	SphereCollider();

	void Init(GameObject * containingObject, float radius, glm::vec4 center, bool trigger = false, float weight = 1);

	CollisionState ApplyForce(glm::vec3 force);

	CollisionState DetectCollision();

	void setMinMax(float xMin, float xMax, float yMin, float yMax);

	bool gotOutBoundaries();

	void Start();

	void Update();

	std::string m_CollidedObjectName;
	std::string m_CollidedObjectTag;

	bool m_active;

private:
	static std::vector<SphereCollider*>m_PhysicsComponents;
	float m_radius;
	glm::vec4 m_center;
	float m_weight;
	bool m_Trigger;

	glm::vec2 m_Xminmax;
	glm::vec2 m_Yminmax;
};

#endif