#include "Sphere.h"
#include <iostream>
//Sphere takes in rigid body parameters and radius and sets moment
Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour):Rigidbody(ShapeType::SPHERE,position, velocity, 0.0f, mass), m_radius(radius), m_colour(colour)
{
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

Sphere::~Sphere()
{
}
//Update local X and Y for rotation
void Sphere::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::fixedUpdate(gravity, timeStep);

	//store the local ax
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);

	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));

}
//Draw circle and line to show rotation
void Sphere::makeGizmo()
{
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 32, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(0, 0, 0, 1));
}
//Used to now check if clicked on, pOther being a small sphere
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
//Debug info, changed a lot 
void Sphere::debug()
{
	std::cout << "P = " << glm::length(m_velocity * m_mass); 
}
//Tell obj to check collision with spehre
void Sphere::Collide(PhysicsObject * obj)
{
	obj->CollideWithSphere(this);
}
//Check collision with Plane
void Sphere::CollideWithPlane(Plane * obj)
{
	//this  SPHERE
	//Obj   PLANE
	//Planes normal 
	glm::vec2 collisionNormal = obj->getNormal();
	float sphereToPlane = glm::dot(this->getPosition(), obj->getNormal()) - obj->getDistance();
	// if sphere on other side flip normal 
	if (sphereToPlane < 0) {
		collisionNormal *= -1;
		sphereToPlane *= -1;
	}
	//Has it touched
	float intersection = this->getRadius() - sphereToPlane;
	//What way is it going Velocity into place
	float vIntoPlane = glm::dot(m_velocity, collisionNormal);
	//Check if it has collided
	if (intersection > 0) {
		//COLLIDES
		//Contact force
		this->setPosition(this->getPosition() + collisionNormal * (this->getRadius() - sphereToPlane));
		//Is going into the plane resolve collisions
		if (vIntoPlane < 0) {
			glm::vec2 con = this->getPosition() + (collisionNormal * -this->getRadius());
			obj->resolveCollision(this, con);
		}
	}
}
//Sphere v Sphere collision check
void Sphere::CollideWithSphere(Sphere * obj)
{
	//check collision 
	glm::vec2 delta = obj->getPosition() - this->getPosition();
	float distance = glm::length(delta);//Dist between the two
	float intersection = this->getRadius() + obj->getRadius() - distance; //Distnace from radius of the two
	//Has collided
	if(intersection > 0){
		//Contact forces
		glm::vec2 contactForce = 0.5f * (distance - (this->getRadius() + obj->getRadius()))*delta / distance;
		//Kinematic checks
		if (!this->isKinematic() && !obj->isKinematic()) {
			this->setPosition(this->getPosition() + contactForce);
			obj->setPosition(obj->getPosition() - contactForce);
		}
		else if (!this->isKinematic()) {
			this->setPosition(this->getPosition() + (contactForce *2.0f));
		}
		else if (!obj->isKinematic()) {
			obj->setPosition(obj->getPosition() - (contactForce * 2.0f));
		}
		//Resolve collision
		resolveCollisions(obj, 0.5f * (getPosition() + obj->getPosition()));;
	}
}
//Box has the collides with sphere code
void Sphere::CollideWithBox(Box * obj)
{
	obj->CollideWithSphere(this);
}
