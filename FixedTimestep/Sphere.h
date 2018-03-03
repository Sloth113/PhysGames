#pragma once
#include "Rigidbody.h"
#include "Plane.h"
#include "Box.h"	
//Sphere rigid body
//Has a radius and colour
class Sphere :	public Rigidbody
{
public:
	//Takes in rigid body info and radius
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	~Sphere();
	//Scene loop functions
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);
	virtual void debug();
	//Info getters 
	virtual glm::vec2 toWorld(glm::vec2 contact) { return m_position +m_localX * contact.x + m_localY * contact.y; }
	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }
	//Virtual colliion function, implemented here
	virtual void Collide(PhysicsObject* obj);
	virtual void CollideWithPlane(Plane* obj);
	virtual void CollideWithSphere(Sphere* obj);
	virtual void CollideWithBox(Box* obj);

protected:
	float m_radius;
	glm::vec4 m_colour;

};

