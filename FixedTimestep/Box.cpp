#include "Box.h"


Box::Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec2 size, glm::vec4 colour) : Rigidbody(ShapeType::BOX, position, velocity, rotation, mass), m_size(size), m_colour(colour)
{
}

Box::~Box()
{
}

void Box::makeGizmo()
{
	//Transform matrix
	glm::mat4 mat = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	//Rotatino in radians
	mat = glm::rotate(mat, m_rotation * 3.1415926535897f / 180.0f , glm::vec3(0, 0, 1));
	aie::Gizmos::add2DAABBFilled(m_position, m_size /2.0f , m_colour, &mat);
	
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

}

void Box::CollideWithSphere(Sphere * obj)
{
	//Point on edge of sphere and treat this as 4 lines(planes)
}

void Box::CollideWithBox(Box * obj)
{
	//4 v 4 Planes

}
