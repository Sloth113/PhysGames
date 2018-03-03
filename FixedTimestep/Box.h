#pragma once
#include <list>

#include "RigidBody.h"
#include "Plane.h"
#include "Sphere.h"
//
//Box is an orientated 2D box taking rigid body parameters, position, velocity, roatation, mass, colour. 
//Size input is for extents 1/2 Width and Height. 
//
//
class Box :	public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec2 size, glm::vec4 colour);
	~Box();
	//Virtual update fucntions implemented in this class
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);
	virtual glm::vec2 toWorld(glm::vec2 contact) { return m_position + m_localX * contact.x + m_localY * contact.y; }
	//Getter/Setters 
	glm::vec2 getSize() { return m_extents; }
	float getWidth() {	return m_extents.x * 2;	}
	float getHeight() { return m_extents.y * 2; }
	glm::vec4 getColour() { return m_colour; }
	//Collision virtuals
	virtual void Collide(PhysicsObject* obj);
	virtual void CollideWithPlane(Plane* obj);
	virtual void CollideWithSphere(Sphere* obj);
	virtual void CollideWithBox(Box* obj);
	//Check corneres used in Collides with Box function. 
	//Takes in the box its checking againsts a refernece to variables to set. 
	bool checkBoxCorners(const Box& box, glm::vec2& contact, int & numContacts, float &pen, glm::vec2& edgeNormal);

protected:
	glm::vec4 m_colour; //Colour of box 
	glm::vec2 m_extents; //1/2 width height 
	
};

