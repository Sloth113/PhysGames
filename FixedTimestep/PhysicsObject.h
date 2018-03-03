#pragma once
#include <glm\ext.hpp>
#include <Gizmos.h>
//Forward declaring for virtal functions
class Plane;
class Sphere;
class Box;
//Enum for type of shapes there are
enum ShapeType {
	JOINT = -1,
	PLANE = 0,
	SPHERE,
	BOX,
	SHAPE_COUNT
};
//Abstract class 
//Has info on if its alive, what shape it is and virtal classes that need to be implemented
class PhysicsObject
{
protected:
	//Constructor
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID), m_dead(false) {}
public:
	//Virtuals used in update loop for physics scene
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};//Never used
	//Setters/Getters for few variables
	int getShapeID() {
		return (int)m_shapeID;
	}
	void setDestroy() {
		m_dead = true;
	}
	bool isDead() {
		return m_dead;
	}
	//Virtuals for Collision resolutions.
	//Collide is called then collide calls correct Collide with X function
	virtual void Collide(PhysicsObject* obj) = 0;
	virtual void CollideWithPlane(Plane* obj) = 0;
	virtual void CollideWithSphere(Sphere* obj) = 0;
	virtual void CollideWithBox(Box* obj) = 0;
protected:
	ShapeType m_shapeID; //What shape it is 
	bool m_dead;//Does it need to be removed from list 
};

