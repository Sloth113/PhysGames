#include "Sphere.h"
#include <iostream>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour):Rigidbody(ShapeType::SPHERE,position, velocity, 0.0f, mass), m_radius(radius), m_colour(colour)
{
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 32, m_colour);
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	//OLD
	Sphere * other = dynamic_cast<Sphere*>(pOther);
	
	if (other != NULL) {
		//worked
		float dist = glm::distance(m_position, other->getPosition());
		if (dist < (m_radius + other->getRadius())) {
			return true;
		}
	}
	else {
		return false;
	}
	return false;
}

void Sphere::Collide(PhysicsObject * obj)
{
	obj->CollideWithSphere(this);
}

void Sphere::CollideWithPlane(Plane * obj)
{
	//this  SPHERE
	//Obj   PLANE
	
	//Null ptr check

	glm::vec2 collisionNormal = obj->getNormal();
	float sphereToPlane = glm::dot(this->getPosition(), obj->getNormal()) - obj->getDistance();
	// if sphere on other side
	if (sphereToPlane < 0) {
		collisionNormal *= -1;
		sphereToPlane *= -1;
	}
	//Has it touched
	float intersection = this->getRadius() - sphereToPlane;
	//What way is it going Velocity into place
	float vIntoPlane = glm::dot(m_velocity, collisionNormal);

	if (intersection > 0) {
		//COLLIDES
		//this->m_velocity = glm::vec2(0, 0);
		if(vIntoPlane < 0)
			obj->resolveCollision(this);
	}

}

void Sphere::CollideWithSphere(Sphere * obj)
{
	//check collision 
	float dist = glm::distance(m_position, obj->getPosition());

	if (dist < m_radius + obj->getRadius()) {
		//Collided
		//m_velocity = glm::vec2(0, 0);
		//obj->m_velocity = glm::vec2(0, 0);
		//
		
		resolveCollisions(obj);
	}
}

void Sphere::CollideWithBox(Box * obj)
{
	obj->CollideWithSphere(this);
}
