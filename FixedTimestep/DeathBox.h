#pragma once
#include "Box.h"
//This is a box that when something collides with it, it will set the object to be destroyed //Could add a kill count 
class DeathBox :
	public Box
{
public:
	DeathBox(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec2 size, glm::vec4 colour);
	~DeathBox();
	virtual void CollideWithSphere(Sphere* obj);
	virtual void CollideWithBox(Box* obj);
};

