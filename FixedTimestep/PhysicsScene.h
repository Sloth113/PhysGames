#pragma once
#include <iostream>
#include <glm\ext.hpp>
#include <vector>
#include <list>
#include "Rigidbody.h"
#include "Spring.h"
#include "PhysicsObject.h"


class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();
	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float dt);
	void updateGizmos();
	void debugScene();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void checkForCollision(); 
	std::vector<PhysicsObject*> getActors() { return m_actors; }
	
	//static bool plane2Plane(PhysicsObject*, PhysicsObject*); 
	//static bool plane2Sphere(PhysicsObject*, PhysicsObject*); 
	//static bool sphere2Plane(PhysicsObject*, PhysicsObject*); 
	//static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

};

