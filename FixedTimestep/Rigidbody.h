#pragma once
#include "PhysicsObject.h"
#define MIN_LINEAR_THRESHOLD 0.01f
#define MIN_ROTATION_THRESHOL 0.01f

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force);
	void applyForceToActor(Rigidbody* actor2, glm::vec2 force);
	void setStatic(bool set);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;
	void resolveCollisions(Rigidbody* actor2);

	glm::vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	float getLinearDrag() { return m_linearDrag; }
	float getAngularDrag() { return m_angularDrag; }
	float getElasticity() { return m_elasticity; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_angularVelocity;
	float m_moment;
	float m_mass;
	float m_rotation;	//2D so we only need a single float to represent our rotation //MAKE RADIANS
	float m_linearDrag;
	float m_angularDrag;
	float m_elasticity;
	bool m_static = false;
};

