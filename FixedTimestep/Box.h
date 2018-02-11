#pragma once
#include "RigidBody.h"
#include "Plane.h"
#include "Sphere.h"
class Box :	public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec2 size, glm::vec4 colour);
	~Box();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	glm::vec2 getSize() { return m_size; }
	glm::vec4 getColour() { return m_colour; }

	virtual void Collide(PhysicsObject* obj);
	virtual void CollideWithPlane(Plane* obj);
	virtual void CollideWithSphere(Sphere* obj);
	virtual void CollideWithBox(Box* obj);

protected:
	glm::vec2 m_size;
	glm::vec4 m_colour;

};

