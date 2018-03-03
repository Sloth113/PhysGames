#include "Box.h"
#include <iostream>
//Constructor, calculates moment, local x and y, points 
Box::Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec2 size, glm::vec4 colour) : Rigidbody(ShapeType::BOX, position, velocity, rotation, mass), m_extents(size), m_colour(colour)
{
	m_moment = 1.0f / 12.0f * m_mass * m_extents.x * 2 * m_extents.y * 2;
	m_localX = glm::normalize(glm::vec2(1, 0));
	m_localY = glm::normalize(glm::vec2(1, 0));
	m_angularVelocity = 0;
}

Box::~Box()
{
}
//Calls rigid body update and Calculates the cos and sin 
void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::fixedUpdate(gravity, timeStep);
	//store the local ax
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	//Set locals (normalised) //glm::normalize
	m_localX = (glm::vec2(cs, sn));
	m_localY = (glm::vec2(-sn, cs));	
}
//Draws two triangles using 4 points calculated 
void Box::makeGizmo()
{
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y; 
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y; 
	
	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}
//Check collision is used a check inside for mouse clicks, was used to check for sphere collision, passing in a tiny sphere will check if its inside
bool Box::checkCollision(PhysicsObject * pOther)
{	
	//Casts to sphere check if worked
	Sphere * obj = dynamic_cast<Sphere*>(pOther);

	if (obj != NULL) {
		//Sphere to box collision code // Relative position
		glm::vec2 circlePos = obj->getPosition() - this->getPosition();
		float w2 = this->getWidth() / 2, h2 = this->getHeight() / 2; //Width and Height extents
		
		 // check the four corners to see if any of them are inside the circle
		for (float x = -w2; x <= w2; x += this->getWidth()) {
			for (float y = -h2; y <= h2; y += this->getHeight()) {
				glm::vec2 p = x * m_localX + y * m_localY; //box point 
				glm::vec2 dp = p - circlePos; //circle point ralitvie to corner bit 
				if (dp.x*dp.x + dp.y*dp.y <= obj->getRadius()*obj->getRadius()) {
					//Contact 
					return true;
				}
			}
		}

		// get the local position of the circle centre
		glm::vec2 localPos(glm::dot(m_localX, circlePos), glm::dot(m_localY, circlePos));
		//Check each side of box
		if (localPos.y < h2 && localPos.y > -h2) {
			if (localPos.x > 0 && localPos.x < w2 + obj->getRadius()) {
				return true;
			}
			if (localPos.x < 0 && localPos.x > -(w2 + obj->getRadius())) {
				return true;
			}
		}
		if (localPos.x < w2 && localPos.x > -w2) {
			if (localPos.y > 0 && localPos.y < h2 + obj->getRadius()) {
				return true;
			}
			if (localPos.y < 0 && localPos.y > -(h2 + obj->getRadius())) {
				return true;
			}
		}
		return false;	
	}
}
//Virtal function table look up
void Box::Collide(PhysicsObject * obj)
{
	obj->CollideWithBox(this);
}
//Collision check for a plane
void Box::CollideWithPlane(Plane * obj)
{
	//Do points of each corner
	//if we are successful then test for collision
	int numContacts = 0;
	glm::vec2 contact(0, 0);
	float contactV = 0;
	float radius = 0.5f * std::fminf(getWidth(), getHeight());
	float penetration = 0;

	// which side is the centre of mass on
	glm::vec2 planeOrigin = obj->getNormal() * obj->getDistance();//relative pos
	float comFromPlane = glm::dot(m_position - planeOrigin, obj->getNormal()); //

	// check all four corners to see if we've hit the plane
	for (float x = -m_extents.x; x < m_extents.x * 2; x += m_extents.x * 2) {
		for (float y = -m_extents.y; y < m_extents.y * 2; y += m_extents.y * 2) {
			// get the position of the corner in world space
			glm::vec2 p = m_position + x * m_localX + y * m_localY;
			//
			float distFromPlane = glm::dot(p - planeOrigin, obj->getNormal());
			// this is the total velocity of the point
			float velocityIntoPlane = glm::dot(m_velocity + m_angularVelocity * (-y * m_localX + x * m_localY), obj->getNormal());
			// if this corner is on the opposite side from the COM,
			// and moving further in, we need to resolve the collision
			if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) || (distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0)) {
				numContacts++;
				contact += p;
				contactV += velocityIntoPlane;
				//
				if (comFromPlane >= 0) {
					if (penetration > distFromPlane)
						penetration = distFromPlane;
				}
				else {
					if (penetration < distFromPlane)
						penetration = distFromPlane;
				}
			}
		}
	}
	// the is a collision 
	if (numContacts > 0) {
		// get the average collision velocity into the plane
		float collisionV = contactV / (float)numContacts;

		// get the acceleration required to stop (restitution = 0) or reverse
		// (restitution = 1) the average velocity into the plane
		glm::vec2 acceleration = -obj->getNormal() *((1.0f + m_elasticity) * collisionV);

		//The average position to apply the force
		glm::vec2 localContact = (contact / (float)numContacts) - m_position;
		// Perpendicular distance we apply the force at relative to
		float r = glm::dot(localContact, glm::vec2(obj->getNormal().y, -obj->getNormal().x));

		// mass of box calculted
		float mass0 = 1.0f / (1.0f / getMass() + (r*r) / getMoment());

		applyForce(acceleration * mass0, localContact);
		//Contact force if this is not kinematic move out of plane using normal and penetration 
		if(!this->isKinematic())
			this->setPosition(this->getPosition() - obj->getNormal() * penetration);
	}
}
//Collide Box with Sphere
void Box::CollideWithSphere(Sphere * obj)
{
	//Circle position 
	glm::vec2 circlePos = obj->getPosition() - this->getPosition();
	float w2 = m_extents.x, h2 = m_extents.y; //Extents
	//Vars used to calc
	int numContacts = 0;
	glm::vec2 contact(0, 0); // contact is in our box coordinates
	float pen = 0;
	glm::vec2* direction = nullptr;
	glm::vec2 penDir = glm::vec2(0, 0);
	 // check the four corners to see if any of them are inside the circle
	for (float x = -w2; x <= w2; x += this->getWidth()) {
		for (float y = -h2; y <= h2; y += this->getHeight()) {
			glm::vec2 p = x * m_localX + y * m_localY;//Corner position 
			glm::vec2 dp = p - circlePos; //Relative position
			if (dp.x*dp.x + dp.y*dp.y <= obj->getRadius()*obj->getRadius()) {
				//Penatration into circle. 
				pen = -(obj->getRadius() - glm::length(p - circlePos));
				penDir = (glm::normalize(dp)); //direction to push out
				
				numContacts++;
				contact += glm::vec2(x,y);//add to contact point
			}
		}
	}

	// get the local position of the circle centre
	glm::vec2 localPos(glm::dot(m_localX, circlePos), glm::dot(m_localY, circlePos));
	//Check each section inside the box, split into 4 parts
	//Get smallest pen and make it the side it goes out. 
	//Uses glm abs as dealing with negative pen on sides left/bottom
	if (localPos.y < h2 && localPos.y > -h2) {
		//right side
		if (localPos.x > 0 && localPos.x < w2 + obj->getRadius()) {
			float pen0 =(w2 + obj->getRadius() - localPos.x);
			if (glm::abs(pen0) < pen || pen == 0){
				pen = pen0;
				numContacts++;
				direction = new glm::vec2(m_localX);
				penDir = glm::vec2(m_localX);
				contact += glm::vec2(w2, localPos.y);
			}
		}
		//left side
		if (localPos.x < 0 && localPos.x > -(w2 + obj->getRadius())) {
			float pen0 = -(w2 + obj->getRadius()) - localPos.x;
			if (glm::abs(pen0) < pen || pen == 0) {
				pen = -pen0;
				numContacts++;
				direction = new glm::vec2(-m_localX);
				penDir = glm::vec2(-m_localX);
				contact += glm::vec2(-w2, localPos.y);
			}
		}
	}
	if (localPos.x < w2 && localPos.x > -w2) {
		//top half
		if (localPos.y > 0 && localPos.y < h2 + obj->getRadius()) {
			float pen0 = h2 + obj->getRadius() - localPos.y;
			if (glm::abs(pen0) < pen || pen == 0) {
				pen = pen0;
				numContacts++;
				direction = new glm::vec2(m_localY);
				penDir = glm::vec2(m_localY);
				contact += glm::vec2(localPos.x, h2);
			}
		}
		//bottom half
		if (localPos.y < 0 && localPos.y > -(h2 + obj->getRadius())) {
			float pen0 = -(h2 + obj->getRadius()) - localPos.y;
			if (glm::abs(pen0) < pen || pen == 0) {
				pen = -pen0;
				numContacts++;
				direction = new glm::vec2(-m_localY);
				penDir = glm::vec2(-m_localY);
				contact += glm::vec2(localPos.x, -h2);
			}

		}
	}
	//If there is a contact
	if(pen != 0){
		//Convert back to world coord
		contact = m_position + (1.0f / numContacts) * (m_localX *contact.x + m_localY * contact.y);
		glm::vec2 penVec = (penDir) * (- pen);
		//Contact forces and then resolve collision
		if (!this->isKinematic() && !obj->isKinematic()) {
			//Both moveable
			this->setPosition(this->getPosition() + penVec*0.5f);
			obj->setPosition(obj->getPosition() - penVec*0.5f); 
		} else if (!this->isKinematic()) {
			//Other is kinematic
			this->setPosition(this->getPosition() + penVec); 
		} else if (!obj->isKinematic()){ 
			//this is kinematic
			obj->setPosition(obj->getPosition() - penVec); 
		}
		resolveCollisions(obj, contact, direction);
	}
	delete direction;
}
//Box v Box collision
void Box::CollideWithBox(Box * obj)
{
	//Relative position 
	glm::vec2 boxPos = obj->getPosition() - this->getPosition();

	glm::vec2 norm(0, 0);
	glm::vec2 contact(0, 0); 
	float pen = 0; 
	int numContacts = 0;
	//Check corners with box passing in variables by reference
	this->checkBoxCorners(*obj, contact, numContacts, pen, norm);
	//Check other box's corners with thing on, reverse normal if they are colliding
	if (obj->checkBoxCorners(*this, contact, numContacts, pen, norm)) {
		norm = -norm;
	}
	//There is a collision 
	if (pen > 0) {
		//Check kinematics
		if (!this->isKinematic() && !obj->isKinematic()) {
			//Both moveable
			this->setPosition(this->getPosition() - (norm * pen * 0.5f));
			obj->setPosition(obj->getPosition() + norm * pen * 0.5f);
		}
		else if (!this->isKinematic()) {
			//This moveable
			this->setPosition(this->getPosition() - (norm * pen));
		}
		else if(!obj->isKinematic()) {
			//Other box is moveable this is kinematic
			obj->setPosition(obj->getPosition() + norm * pen);
		}
		//
		this->resolveCollisions(obj, contact / float(numContacts), &norm);
		}
}

//Check box corners, takes in box to check against and variables by reference to return info
bool Box::checkBoxCorners(const Box & box, glm::vec2 & contact, int & numContacts, float & pen, glm::vec2 & edgeNormal)
{
	//Vars used
	float minX, maxX, minY, maxY;
	float boxW = box.m_extents.x * 2;
	float boxH = box.m_extents.y * 2;
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	float penetration = 0;

	bool first = true;
	//Check corners
	for (float x = -box.m_extents.x; x < boxW; x += boxW){
		for (float y = -box.m_extents.y; y < boxH; y += boxH){
			// position in worldspace
			glm::vec2 p = box.m_position + x * box.m_localX + y * box.m_localY;
			// position in our box's space
			glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));
			float w2 = m_extents.x, h2 = m_extents.y; //Extents
			//First collision or position in boxs space is in its space
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;
			//If any of the points in boxes space overlap extants
			if (p0.x >= -m_extents.x && p0.x <= m_extents.x && p0.y >= -m_extents.y && p0.y <= m_extents.y) {
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}
	}
	//If the positions are outside of the extents //No contact return
	if (maxX <= -m_extents.x || minX >=m_extents.x || maxY<=-m_extents.y || minY >=m_extents.y)
		return false;
	if (numLocalContacts == 0)
		return false;

	bool res = false;
	//contact postion
	contact += m_position + (localContact.x*m_localX + localContact.y*m_localY) / (float)numLocalContacts;

	numContacts++;
	//Setting the penetration and edge normals depending on what side is the smallest penetration
	float pen0 = m_extents.x - minX ;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = m_localX;
		pen = pen0;
		res = true;
	}

	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localX;
		pen = pen0; 
		res = true;
	}

	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) { 
		edgeNormal = m_localY; 
		pen = pen0; 
		res = true; 
	}
	pen0 = maxY + m_extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localY;
		pen = pen0;
		res = true; 
	} 
	return res;
}

