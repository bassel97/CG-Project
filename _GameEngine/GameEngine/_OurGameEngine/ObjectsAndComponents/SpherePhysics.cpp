#include"SpherePhysics.hpp"

SphereCollider::SphereCollider(GameObject * containingObject, float radius, glm::vec4 center, bool trigger, float weight) : Component(containingObject)
{
	m_radius = radius;
	m_center = center;
	m_Trigger = trigger;
	m_weight = weight;
	m_PhysicsComponents.push_back(this);

	containingObject->addComponent(this);

	m_active = true;
}

SphereCollider::SphereCollider() : Component(NULL)
{
}

void SphereCollider::Init(GameObject * containingObject, float radius, glm::vec4 center, bool trigger, float weight)
{
	this->containingObject = containingObject;

	m_radius = radius;
	m_center = center;
	m_Trigger = trigger;
	m_weight = weight;
	m_PhysicsComponents.push_back(this);

	containingObject->addComponent(this);

	m_active = true;
}

CollisionState SphereCollider::ApplyForce(glm::vec3 force)
{
	float translationSpeed = (1 / m_weight) * (1 - containingObject->freeze);

	this->containingObject->Translate(force * translationSpeed);
	CollisionState value = DetectCollision();

	bool gotOut = gotOutBoundaries();

	if (value == COLLIDED || gotOut) {
		this->containingObject->Translate(force * (-1 / m_weight));
	}
	return value;
}

CollisionState SphereCollider::DetectCollision()
{
	for (int i = 0; i < m_PhysicsComponents.size(); i++) {
		if (m_PhysicsComponents[i] == this) continue;
		if (!m_PhysicsComponents[i]->m_active)continue;
		glm::vec4 CenterFromOrigin = this->containingObject->getTransformMatrix() * this->m_center;
		glm::vec4 otherCenterFromOrigin = m_PhysicsComponents[i]->containingObject->getTransformMatrix() * m_PhysicsComponents[i]->m_center;

		//std::cout << CenterFromOrigin.x << ", " << CenterFromOrigin.y << ", " << CenterFromOrigin.z << std::endl;
		float distance = glm::distance(CenterFromOrigin, otherCenterFromOrigin);
		if (distance <= this->m_radius + m_PhysicsComponents[i]->m_radius) {
			if (m_PhysicsComponents[i]->m_Trigger) {
				m_CollidedObjectName = m_PhysicsComponents[i]->containingObject->name;
				m_CollidedObjectTag = m_PhysicsComponents[i]->containingObject->tag;
				m_CollidedObject = m_PhysicsComponents[i]->containingObject;
				return TRIGGERED;
			}
			else {
				m_CollidedObjectName = m_PhysicsComponents[i]->containingObject->name;
				m_CollidedObjectTag = m_PhysicsComponents[i]->containingObject->tag;
				m_CollidedObject = m_PhysicsComponents[i]->containingObject;
				return COLLIDED;
			}
		}
	}
	return NOT_COLLIDED;
}

void SphereCollider::setMinMax(float xMin, float xMax, float yMin, float yMax)
{
	m_Xminmax.x = xMin;
	m_Xminmax.y = xMax;
	m_Yminmax.x = yMin;
	m_Yminmax.y = yMax;
}

bool SphereCollider::gotOutBoundaries()
{
	glm::vec3 pos = this->containingObject->getPosition();
	if (pos.x > m_Xminmax.y)
		return true;
	if (pos.x < m_Xminmax.x)
		return true;
	if (pos.z > m_Yminmax.y)
		return true;
	if (pos.z < m_Yminmax.x)
		return true;

	return false;
}

void SphereCollider::Start()
{

}

void SphereCollider::Update()
{
}

void SphereCollider::UpdateWeight(float scale)
{
	if (scale != 0)
		m_weight *= scale;
}
