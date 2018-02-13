#include "Rigidbody.h"



Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) :PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_angularVelocity(0), m_linearDrag(0.3f), m_angularDrag(0.3f), m_elasticity(1), m_moment(1)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (!m_static) {
		if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD) {
			m_velocity = glm::vec2(0, 0);
		}
		if (glm::abs(m_angularVelocity) < MIN_ROTATION_THRESHOL) {
			m_angularVelocity = 0;
		}

		m_position += m_velocity * timeStep;
		//m_velocity += gravity * timeStep;
		applyForce(gravity * m_mass * timeStep, glm::vec2(0,0));

		m_velocity -= m_velocity * m_linearDrag * timeStep;
		m_rotation += m_angularVelocity * timeStep;
		m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;


	}
	else {
		m_velocity = glm::vec2(0, 0);
	}
}

void Rigidbody::debug()
{

}

void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / (m_moment);
}
/*
void Rigidbody::applyForceToActor(Rigidbody * actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}
*/
void Rigidbody::setStatic(bool set)
{
	m_static = set;
	m_mass = 9999999999;
}

void Rigidbody::resolveCollisions(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal) 
{
	// find the vector between their centres, or use the provided direction // of force
	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->m_position - m_position);

	// get the vector perpendicular to the collision normal 
	glm::vec2 perp(normal.y, -normal.x);

	// detemine the total velocity of the contact points for the two objects, // for both linear and rotational

	// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp); 
	float r2 = glm::dot(contact - actor2->m_position, perp);
	// velocity of the contact point on this object 
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// velocity of contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2) // they're moving closer
	{
		// calculate the effective mass at contact point for each object 
		// ie how much the contact point will move due to the force applied.
		float mass1 = 1.0f / (1.0f / m_mass + (r1*r1) / m_moment); 
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2*r2) / actor2->m_moment); 

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f; 
		glm::vec2 force = (1.0f + elasticity)*mass1*mass2 / (mass1 + mass2)*(v1 - v2)*normal; 
		//apply equal and opposite forces
		applyForce(-force, contact - m_position); 
		actor2->applyForce(force, contact - actor2->m_position);
	}

}
/*
void Rigidbody::resolveCollisions(Rigidbody * actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;
	if (glm::dot(relativeVelocity, normal) > 0) return; //Stops being stuck
	float elasticity = (m_elasticity + actor2->getElasticity())/2.0f; 
	float j = glm::dot(-(1.0f + elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * ((1.0f / m_mass) + (1.0f / actor2->getMass())));
	//if (m_static) {
		//j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * (1 / actor2->getMass())); // this is static
	//}
	//else if (actor2->m_static) {
		//j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * (1 / m_mass)); //other is static
	//}
	glm::vec2 force = normal * j; 
	applyForceToActor(actor2, force);
}*/
