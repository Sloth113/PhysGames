#include "Plane.h"

//Default plane constructor makes and line on buttom of screen 
Plane::Plane():PhysicsObject(ShapeType::PLANE), m_distanceToOrigin(0), m_normal(glm::vec2(0,1)), m_elasticity(1)
{
}
//Sets vars 
Plane::Plane(glm::vec2 normal, float distance):PhysicsObject(ShapeType::PLANE), m_distanceToOrigin(distance)
{
	m_normal = glm::normalize(normal); //Makes sure nomal is normalised
}

Plane::~Plane()
{
}
//Does not change on updates
void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{

}
//Draw the line using draw 2d line, line segment length in how big it will be on screen 
void Plane::makeGizmo()
{
	float lineSegmentLength = 10000; //large because
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin; 
	// easy to rotate normal through 90 degrees around z 
	glm::vec2 parallel(m_normal.y, -m_normal.x); 
	glm::vec4 colour(1, 1, 1, 1); 
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength); 
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength); 
	aie::Gizmos::add2DLine(start, end, colour);
}
//Does nothing
void Plane::resetPosition()
{
}
//Old outdated, trats actor 2 as sphere
void Plane::resolveCollision(Rigidbody * actor2, glm::vec2 contact)
{
	glm::vec2 normal = m_normal;// glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity();
	
	float elasticity = m_elasticity;//CHANGE
	float j = (-(1 + elasticity) * glm::dot(relativeVelocity, normal)) / glm::dot(normal, normal * ((1 / actor2->getMass())));
	glm::vec2 force = normal * j;
	actor2->applyForce(force, contact - actor2->getPosition());
}
//Current colision, tell whatever to check if its collided with this plane
void Plane::Collide(PhysicsObject * obj)
{
	obj->CollideWithPlane(this);
}
//Nothing both static objects
void Plane::CollideWithPlane(Plane * obj)
{
	//Nothing happens
}
//Sphere contains the collision code
void Plane::CollideWithSphere(Sphere * obj)
{
	obj->CollideWithPlane(this);
}
//Box contains the collision code
void Plane::CollideWithBox(Box * obj)
{
	obj->CollideWithPlane(this);
}
