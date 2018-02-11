#include "Line.h"


Line::Line(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float length, glm::vec4 colour) :Rigidbody(ShapeType::PLANE, position, velocity, rotation, mass), m_length(length), m_colour(colour)
{
}

Line::~Line()
{
}

void Line::makeGizmo()
{
	glm::vec2 pos1 = glm::vec2(0, 0); 
	glm::vec2 pos2 = m_position;
	
	aie::Gizmos::add2DLine(pos1, pos2, m_colour);
}

bool Line::checkCollision(PhysicsObject * pOther)
{
	return false;
}

void Line::Collide(PhysicsObject * obj)
{
}

void Line::CollideWithPlane(Plane * obj)
{
}

void Line::CollideWithSphere(Sphere * obj)
{
}

void Line::CollideWithBox(Box * obj)
{
}
