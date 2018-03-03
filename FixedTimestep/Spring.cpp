#include "Spring.h"

//Spring takes in the bodies it attached to, the rest length, how springy it is and dampign, the contact poitns for each body
Spring::Spring(Rigidbody * body1, Rigidbody * body2, float restLength, float springCoefficient, float damping, glm::vec2 contact1, glm::vec2 contact2):PhysicsObject(ShapeType::JOINT), m_body1(body1), m_body2(body2), m_restLength(restLength), m_springCoefficient(springCoefficient), m_damping(damping), m_contact1(contact1), m_contact2(contact2)
{
}

Spring::~Spring()
{
}
//Update applies forces to the bodies to move them closer to the resting postion
void Spring::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	//If one of the attached bodies is to die so is this and dont update
	if (m_body1->isDead() || m_body2->isDead()) {
		m_dead = true; 
	}
	if (!m_dead) {
		//Contact points 
		glm::vec2 p2 = m_body2->toWorld(m_contact2);
		glm::vec2 p1 = m_body1->toWorld(m_contact1);
		glm::vec2 dist = p2 - p1;
		//
		float length = sqrtf(dist.x*dist.x + dist.y* dist.y);

		// apply damping 
		glm::vec2 relativeVelocity = m_body2->getVelocity() - m_body1->getVelocity();
		// Change in length
		float dlength = (m_restLength - length);
		//If the change in length is large set it rest length
		if (dlength > m_restLength /**4*/) {
			dlength = m_restLength;
		}
		if (dlength < -m_restLength /**4*/) {
			dlength = -m_restLength;
		}
		//How much force is required
		glm::vec2 force = dist * m_springCoefficient * dlength - m_damping * relativeVelocity;

		m_body1->applyForce(-force * timeStep, p1 - m_body1->getPosition());
		m_body2->applyForce(force * timeStep, p2 - m_body2->getPosition());
	}
}

void Spring::debug()
{
}
//Draw a grey line to show spring
void Spring::makeGizmo()
{
	if(!m_dead)
		aie::Gizmos::add2DLine(m_body1->toWorld(m_contact1), m_body2->toWorld(m_contact2), glm::vec4(0.5, 0.5, 0.5, 1));
}

