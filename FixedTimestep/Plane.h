#pragma once
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Box.h"
//Plane physics object is an infinite line that has a normal and a distance from origin. 
class Plane : public PhysicsObject
{
public:
	Plane(); 
	Plane(glm::vec2 normal, float distance); 
	~Plane(); 
	//Implements virtual functions to become a class
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {} 
	virtual void makeGizmo(); 
	virtual void resetPosition(); 
	//Getters//Setters
	glm::vec2 getNormal() { return m_normal; } 
	float getDistance() { return m_distanceToOrigin; } 
	float getElasticity() { return m_elasticity; }
	void setElasticity(float set) { m_elasticity = set; }

	void resolveCollision(Rigidbody * actor2, glm::vec2 contact);//old

	//Virtual Collision functions 
	virtual void Collide(PhysicsObject* obj);
	virtual void CollideWithPlane(Plane* obj);
	virtual void CollideWithSphere(Sphere* obj);
	virtual void CollideWithBox(Box* obj);

protected: 
	glm::vec2 m_normal; 
	float m_distanceToOrigin;
	float m_elasticity = 1;

};

