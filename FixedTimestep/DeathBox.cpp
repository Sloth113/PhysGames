#include "DeathBox.h"


//Normal box constructor// sets kinematic as we dont want it to destroy the scene
DeathBox::DeathBox(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec2 size, glm::vec4 colour):Box(position, velocity, rotation, mass, size, colour)
{
	setKinematic(true);//Kinematic by default
}

DeathBox::~DeathBox()
{
}
//Copied collision code from box class, changed so that if it does collide it just sets the other obj to die
void DeathBox::CollideWithSphere(Sphere * obj)
{
	glm::vec2 circlePos = obj->getPosition() - this->getPosition();
	float w2 = this->getWidth() / 2, h2 = this->getHeight() / 2;

	int numContacts = 0;
	glm::vec2 contact(0, 0); // contact is in our box coordinates
	float pen = 0;
	glm::vec2* direction = nullptr;
	glm::vec2 penDir = glm::vec2(0, 0);
	// check the four corners to see if any of them are inside the circle
	for (float x = -w2; x <= w2; x += this->getWidth()) {
		for (float y = -h2; y <= h2; y += this->getHeight()) {
			glm::vec2 p = x * m_localX + y * m_localY;
			glm::vec2 dp = p - circlePos;
			if (dp.x*dp.x + dp.y*dp.y <= obj->getRadius()*obj->getRadius()) {

				pen = -(obj->getRadius() - glm::length(p - circlePos));
				penDir = (glm::normalize(dp));

				numContacts++;
				contact += glm::vec2(x, y);

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
			float pen0 = (w2 + obj->getRadius() - localPos.x);
			if (glm::abs(pen0) < pen || pen == 0) {
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

	if (pen != 0) {
		obj->setDestroy();//Kill instead of collide with things
	}
	delete direction;
}
//Copied code from box class if contact happens set the other box to die
void DeathBox::CollideWithBox(Box * obj) {
	//4 v 4 Planes
	glm::vec2 boxPos = obj->getPosition() - this->getPosition();

	glm::vec2 norm(0, 0);
	glm::vec2 contact(0, 0);
	float pen = 0;
	int numContacts = 0;


	this->checkBoxCorners(*obj, contact, numContacts, pen, norm);
	//this->checkBoxCorners(*obj, contact, numContacts, norm, contactForce1);

	if (obj->checkBoxCorners(*this, contact, numContacts, pen, norm)) {
		norm = -norm;
	}

	if (pen > 0) {
		obj->setDestroy();
	}
}
