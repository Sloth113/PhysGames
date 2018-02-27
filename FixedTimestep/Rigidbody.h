#pragma once
#include "PhysicsObject.h"
#define MIN_LINEAR_THRESHOLD 0.1f
#define MIN_ROTATION_THRESHOL 0.01f

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force, glm::vec2 pos);
	virtual glm::vec2 toWorld(glm::vec2 contact) { return m_position; }
	//void applyForceToActor(Rigidbody* actor2, glm::vec2 force);


	virtual bool checkCollision(PhysicsObject* pOther) = 0;
	void resolveCollisions(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr);

	
	bool isKinematic() { return m_isKinematic; }
	void setKinematic(bool set) { m_isKinematic = set; /*m_mass = (float)INT_MAX; m_moment = (float)INT_MAX;*/} //should change to get Mass and get Moment for dynamically changing kinematics
	glm::vec2 getPosition() { return m_position; }
	void setPosition(glm::vec2 pos) { m_position = pos; }
	float getRotation() { return m_rotation; }
	void setRotation(float set) { m_rotation = set * (3.141592f / 180.0f); }
	glm::vec2 getVelocity() { return m_velocity; }
	void setVelocity(glm::vec2 set) { m_velocity = set; }
	float getMass() { return m_isKinematic ? INT_MAX : m_mass; }
	float getMoment() { return m_isKinematic ? INT_MAX : m_moment; }
	void setMass(float set) { m_mass = set; }
	float getLinearDrag() { return m_linearDrag; }
	void setLinearDrag(float set) { m_linearDrag = set; }
	float getAngularDrag() { return m_angularDrag; }
	void setAngularDrag(float set) { m_angularDrag = set; }
	float getElasticity() { return m_elasticity; }
	void setElasticity(float set) { m_elasticity = set; }

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
	bool m_isKinematic = false;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

