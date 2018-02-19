#pragma once
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Box.h"

class Plane : public PhysicsObject
{
public:
	Plane(); 
	Plane(glm::vec2 normal, float distance); 
	~Plane(); 
	
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {} 
	virtual void makeGizmo(); 
	virtual void resetPosition(); 
	
	glm::vec2 getNormal() { return m_normal; } 
	float getDistance() { return m_distanceToOrigin; } 
	void resolveCollision(Rigidbody * actor2, glm::vec2 contact);

	float getElasticity() { return m_elasticity; }
	void setElasticity(float set) { m_elasticity = set; }

	virtual void Collide(PhysicsObject* obj);
	virtual void CollideWithPlane(Plane* obj);
	virtual void CollideWithSphere(Sphere* obj);
	virtual void CollideWithBox(Box* obj);

protected: 
	glm::vec2 m_normal; 
	float m_distanceToOrigin;
	float m_elasticity;

};

