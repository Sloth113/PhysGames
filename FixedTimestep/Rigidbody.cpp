#include "Rigidbody.h"

//Set variables to input or default, Drag values set to 0, elastisity to 1 and moment to 1 as it will be set by the non abstract classes
Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) :PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_rotation(rotation*(3.141592f / 180.0f)), m_mass(mass), m_angularVelocity(0), m_linearDrag(0.0f), m_angularDrag(0.0f), m_elasticity(1), m_moment(1)
{

}

Rigidbody::~Rigidbody()
{
}
//Update the rigid body to act under gravity and move using its velocity, angular velocity and then drag values
void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (!m_isKinematic) {

		m_position += m_velocity * timeStep;
		//m_velocity += gravity * timeStep;
		applyForce(gravity * getMass() * timeStep, glm::vec2(0,0));

		m_velocity -= m_velocity * m_linearDrag * timeStep;
		m_rotation += m_angularVelocity * timeStep;
		m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

		//Not implemented due to stopping when gravity is involved
		if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD) {
			//if(glm::length(gravity) != 0 && glm::length(m_velocity) < glm::length(gravity) * m_linearDrag * timeStep)
			//	m_velocity = glm::vec2(0, 0);
		}
		if (glm::abs(m_angularVelocity) < MIN_ROTATION_THRESHOL) {
			m_angularVelocity = 0;
		}

	}
	else {
		//Is static, set veloicties to 0 
		m_velocity = glm::vec2(0, 0);
		m_angularVelocity = 0;
		return; 
	}
}

void Rigidbody::debug()
{

}
//Apply force to center and set angular velocity as if it was hit at that position
void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / getMass();
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / (getMoment());
}

/*
//Old collision code used for spehres, apply equal and opposite forces
void Rigidbody::applyForceToActor(Rigidbody * actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}
*/
//Resolve a collision between two actors with a contact and normal
void Rigidbody::resolveCollisions(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal) 
{
	// vector between the centers, or use the provided direction // of force
	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->m_position - m_position);

	// perpendicular vector to the collision normal 
	glm::vec2 perp(normal.y, -normal.x);

	// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp); 
	float r2 = glm::dot(contact - actor2->m_position, perp);
	// velocity of the contact point on this object 
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// velocity of contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;
	// if they're moving closer
	if (v1 > v2)
	{
		// calculate the effective mass at contact point for each object 
		float mass1 = 1.0f / (1.0f / getMass() + (r1*r1) / getMoment());
		float mass2 = 1.0f / (1.0f / actor2->getMass() + (r2*r2) / actor2->getMoment());

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;
		glm::vec2 force = (1.0f + elasticity)*mass1*mass2 / (mass1 + mass2)*(v1 - v2)*normal;

		//apply equal and opposite forces
		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->m_position);
	}
}
