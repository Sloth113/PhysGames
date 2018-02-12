#pragma once
#include "Rigidbody.h"
#include "Plane.h"
#include "Box.h"	
class Sphere :	public Rigidbody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	~Sphere();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);
	virtual void debug();
	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }
	

	virtual void Collide(PhysicsObject* obj);
	virtual void CollideWithPlane(Plane* obj);
	virtual void CollideWithSphere(Sphere* obj);
	virtual void CollideWithBox(Box* obj);

protected:
	float m_radius;
	glm::vec4 m_colour;

};

