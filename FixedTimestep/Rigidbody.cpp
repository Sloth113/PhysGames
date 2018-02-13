#include "Rigidbody.h"



Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) :PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_angularVelocity(0), m_linearDrag(0.3f), m_angularDrag(0.3f), m_elasticity(1), m_moment(0)
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
		m_velocity -= m_velocity * m_linearDrag * timeStep;
		m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;
		m_position += m_velocity * timeStep;
		applyForce(gravity * m_mass * timeStep);
	}
	else {
		m_velocity = glm::vec2(0, 0);
	}
}

void Rigidbody::debug()
{

}

void Rigidbody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::applyForceToActor(Rigidbody * actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}

void Rigidbody::setStatic(bool set)
{
	m_static = set;
	m_mass = 9999999999;
}

void Rigidbody::resolveCollisions(Rigidbody * actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;
	if (glm::dot(relativeVelocity, normal) > 0) return; //Stops being stuck
	float elasticity = (m_elasticity + actor2->getElasticity())/2.0f; 
	float j = glm::dot(-(1.0f + elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * ((1.0f / m_mass) + (1.0f / actor2->getMass())));
	if (m_static) {
		//j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * (1 / actor2->getMass())); // this is static
	}
	else if (actor2->m_static) {
//		j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * (1 / m_mass)); //other is static
	}
	glm::vec2 force = normal * j; 
	applyForceToActor(actor2, force);
}
