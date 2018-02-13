#include "Box.h"
#include <iostream>

Box::Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec2 size, glm::vec4 colour) : Rigidbody(ShapeType::BOX, position, velocity, rotation, mass), m_extents(size), m_colour(colour)
{
	m_moment = 1.0f / 12.0f * mass * m_extents.x * 2 * m_extents.y * 2;
	m_localX = glm::normalize(glm::vec2(1, 0));
	m_localY = glm::normalize(glm::vec2(1, 0));
	points = std::list<glm::vec2>();
}

Box::~Box()
{
}

void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	
	Rigidbody::fixedUpdate(gravity, timeStep);
	
	//store the local ax
	float cs = cosf(m_rotation); 
	float sn = sinf(m_rotation); 
	
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
	
}

void Box::makeGizmo()
{
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y; 
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y; 
	
	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
	
}

bool Box::checkCollision(PhysicsObject * pOther)
{
	return false;
}

void Box::Collide(PhysicsObject * obj)
{
	obj->CollideWithBox(this);
}

void Box::CollideWithPlane(Plane * obj)
{
	//Do points of each corner
	//if we are successful then test for collision
	int numContacts = 0;
	glm::vec2 contact(0, 0);
	float contactV = 0;
	float radius = 0.5f * std::fminf(getWidth(), getHeight());

	// which side is the centre of mass on?
	glm::vec2 planeOrigin = obj->getNormal() * obj->getDistance();
	float comFromPlane = glm::dot(m_position - planeOrigin, obj->getNormal());

	// check all four corners to see if we've hit the plane
	for (float x = -m_extents.x; x < m_extents.y * 2; x += m_extents.x * 2) {
		for (float y = -m_extents.y; y < m_extents.y * 2; y += m_extents.y * 2) {
			// get the position of the corner in world space
			glm::vec2 p = m_position + x * m_localX + y * m_localY;


			
			float distFromPlane = glm::dot(p - planeOrigin, obj->getNormal());

			// this is the total velocity of the point
			float velocityIntoPlane = glm::dot(m_velocity + m_angularVelocity * (-y * m_localX + x * m_localY), obj->getNormal());

			// if this corner is on the opposite side from the COM,
			// and moving further in, we need to resolve the collision

			if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) || (distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0)) {
				numContacts++;
				contact += p;
				contactV += velocityIntoPlane;
			}
		}
	}
	// we've had a hit - typically only two corners can contact
	if (numContacts > 0) {
		// get the average collision velocity into the plane
		// (covers linear and rotational velocity of all corners involved)
		float collisionV = contactV / (float)numContacts;

		// get the acceleration required to stop (restitution = 0) or reverse
		// (restitution = 1) the average velocity into the plane
		glm::vec2 acceleration = -obj->getNormal() *((1.0f + m_elasticity) * collisionV);

		// and the average position at which we'll apply the force
		// (corner or edge centre)
		glm::vec2 localContact = (contact / (float)numContacts) - m_position;
		// this is the perpendicular distance we apply the force at relative to
		// the COM, so Torque = F*r
		float r = glm::dot(localContact, glm::vec2(obj->getNormal().y, -obj->getNormal().x));

		// work out the "effective mass" - this is a combination of moment of
		// inertia and mass, and tells us how much the contact point velocity
		// will change with the force we're applying
		float mass0 = 1.0f / (1.0f / m_mass + (r*r) / m_moment);

		applyForce(acceleration * mass0, localContact);
	}
}

void Box::CollideWithSphere(Sphere * obj)
{
	glm::vec2 circlePos = obj->getPosition() - this->getPosition();
	float w2 = this->getWidth() / 2, h2 = this->getHeight() / 2;

	int numContacts = 0;
	glm::vec2 contact(0, 0); // contact is in our box coordinates

							 // check the four corners to see if any of them are inside the circle
	for (float x = -w2; x <= w2; x += this->getWidth()) {
		for (float y = -h2; y <= h2; y += this->getHeight()) {
			glm::vec2 p = x * m_localX + y * m_localY;
			glm::vec2 dp = p - circlePos;
			if (dp.x*dp.x + dp.y*dp.y < obj->getRadius()*obj->getRadius()) {
				numContacts++;
				contact += glm::vec2(x, y);
			}
		}
	}

	glm::vec2* direction = nullptr;
	// get the local position of the circle centre
	glm::vec2 localPos(glm::dot(m_localX, circlePos), glm::dot(m_localY, circlePos));
	
	if (localPos.y < h2 && localPos.y > -h2) {
		if (localPos.x > 0 && localPos.x < w2 + obj->getRadius()) {
			numContacts++;
			contact += glm::vec2(w2, localPos.y);
			direction = new glm::vec2(m_localX);
		}
		if (localPos.x < 0 && localPos.x > -(w2 + obj->getRadius())) {
			numContacts++;
			contact += glm::vec2(-w2, localPos.y);
			direction = new glm::vec2(-m_localX);
		}
	}
	if (localPos.x < w2 && localPos.x > -w2) {
		if (localPos.y > 0 && localPos.y < h2 + obj->getRadius()) {
			numContacts++;
			contact += glm::vec2(localPos.x, h2);
			direction = new glm::vec2(m_localY);
		}
		if (localPos.y < 0 && localPos.y > -(h2 + obj->getRadius())) {
			numContacts++;
			contact += glm::vec2(localPos.x, -h2);
			direction = new glm::vec2(-m_localY);
		}
	}

	if (numContacts > 0) {
		// average, and convert back into world coords
		contact = m_position + (1.0f / numContacts) * (m_localX *contact.x + m_localY*contact.y);

		resolveCollisions(obj, contact, direction);

	}
	delete direction;
}

void Box::CollideWithBox(Box * obj)
{
	//4 v 4 Planes

}
