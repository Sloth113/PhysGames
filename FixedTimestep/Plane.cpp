#include "Plane.h"



Plane::Plane():PhysicsObject(ShapeType::PLANE), m_distanceToOrigin(0), m_normal(glm::vec2(0,1)), m_elasticity(1)
{
}

Plane::Plane(glm::vec2 normal, float distance):PhysicsObject(ShapeType::PLANE), m_distanceToOrigin(distance)
{
	m_normal = glm::normalize(normal);
}


Plane::~Plane()
{
}

void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Plane::makeGizmo()
{
	float lineSegmentLength = 3000; 
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin; 
	// easy to rotate normal through 90 degrees around z 
	glm::vec2 parallel(m_normal.y, -m_normal.x); 
	glm::vec4 colour(1, 1, 1, 1); 
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength); 
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength); 
	aie::Gizmos::add2DLine(start, end, colour);
}

void Plane::resetPosition()
{
}

void Plane::resolveCollision(Rigidbody * actor2, glm::vec2 contact)
{
	glm::vec2 normal = m_normal;// glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity();
	
	float elasticity = m_elasticity;//CHANGE
	float j = (-(1 + elasticity) * glm::dot(relativeVelocity, normal)) / glm::dot(normal, normal * ((1 / actor2->getMass())));
	glm::vec2 force = normal * j;
	actor2->applyForce(force, contact - actor2->getPosition());
}

void Plane::Collide(PhysicsObject * obj)
{
	obj->CollideWithPlane(this);
}

void Plane::CollideWithPlane(Plane * obj)
{
	//Nothing happens
}

void Plane::CollideWithSphere(Sphere * obj)
{
	obj->CollideWithPlane(this);
}

void Plane::CollideWithBox(Box * obj)
{
	obj->CollideWithPlane(this);
}
