#pragma once
#include <glm\ext.hpp>
#include <Gizmos.h>

class Plane;
class Sphere;
class Box;

enum ShapeType {
	JOINT = -1,
	PLANE = 0,
	SPHERE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID), m_dead(false) {}

public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};
	int getShapeID() {
		return (int)m_shapeID;
	}
	void setDestroy() {
		m_dead = true;
	}
	bool isDead() {
		return m_dead;
	}

	virtual void Collide(PhysicsObject* obj) = 0;
	virtual void CollideWithPlane(Plane* obj) = 0;
	virtual void CollideWithSphere(Sphere* obj) = 0;
	virtual void CollideWithBox(Box* obj) = 0;

protected:
	ShapeType m_shapeID;
	bool m_dead;

};

