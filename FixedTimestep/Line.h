#pragma once
#include "Rigidbody.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"

class Line : public Rigidbody
{
public:
	Line(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float length, glm::vec4 colour);
	~Line();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	float getLength() { return m_length; }
	glm::vec4 getColour() { return m_colour; }

	virtual void Collide(PhysicsObject* obj);
	virtual void CollideWithPlane(Plane* obj);
	virtual void CollideWithSphere(Sphere* obj);
	virtual void CollideWithBox(Box* obj);

protected:
	float m_length;
	glm::vec4 m_colour;

};

