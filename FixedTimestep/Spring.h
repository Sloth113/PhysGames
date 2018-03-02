#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"
class Spring : public PhysicsObject
{
public:
	Spring(Rigidbody * body1, Rigidbody * body2, float restLength, float springCoefficient, float damping = 0.1f, glm::vec2 contact1 = glm::vec2(0,0), glm::vec2 contact2 = glm::vec2(0,0));
	~Spring();
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	virtual void makeGizmo();
	bool hasBody(Rigidbody * b) { return b == m_body1 || b == m_body2; }
	Rigidbody * getOther(Rigidbody * b)	
	{
		if		(b = m_body1)	return m_body2;
		else if (b = m_body2)	return m_body1;
		else					return nullptr;
	}
	virtual void Collide(PhysicsObject* obj) {};
	virtual void CollideWithPlane(Plane* obj) {};
	virtual void CollideWithSphere(Sphere* obj) {};
	virtual void CollideWithBox(Box* obj) {};
	
protected:
	Rigidbody * m_body1;
	Rigidbody * m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	float m_damping;
	float m_restLength;
	float m_springCoefficient;
};

