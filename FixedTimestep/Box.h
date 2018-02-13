#pragma once
#include "RigidBody.h"
#include "Plane.h"
#include "Sphere.h"
#include <list>
class Box :	public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec2 size, glm::vec4 colour);
	~Box();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	glm::vec2 getSize() { return m_extents; }
	float getWidth() {	return m_extents.x * 2;	}
	float getHeight() { return m_extents.y * 2; }
	glm::vec4 getColour() { return m_colour; }

	virtual void Collide(PhysicsObject* obj);
	virtual void CollideWithPlane(Plane* obj);
	virtual void CollideWithSphere(Sphere* obj);
	virtual void CollideWithBox(Box* obj);

	bool checkBoxCorners(const Box& box, glm::vec2& contact, int & numContacts, float &pen, glm::vec2& edgeNormal);

protected:
	//glm::vec2 m_size;
	glm::vec4 m_colour;

	glm::vec2 m_extents;
	
	glm::vec2 m_localX;
	glm::vec2 m_localY;

	std::list<glm::vec2> points;

};

