#pragma once
#include <iostream>
#include <glm\ext.hpp>
#include <vector>
#include <list>

#include "Rigidbody.h"
#include "Spring.h"
#include "PhysicsObject.h"

//Class used to run the engine
//Has a list of actors and runs a fixed time step update loop 
class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();
	void addActor(PhysicsObject* actor);//Add actor to scene 
	void removeActor(PhysicsObject* actor); //Remove actor through the physics scene //Not needed but still works // Use ->setDestroy() on actor
	void update(float dt);//update function
	void updateGizmos();//Draw function
	void debugScene();//Used for debug info 
	//Set scenes gravity, 0 by default. 
	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }
	//Set the updates time step
	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }
	//Check collisions 
	void checkForCollision(); 
	//Returns list of actors used for checking clicks or list outside of physics scene
	std::vector<PhysicsObject*> getActors() { return m_actors; }

protected:
	glm::vec2 m_gravity; //Constant force being applied
	float m_timeStep; //Time step
	std::vector<PhysicsObject*> m_actors; //Physic objects

};

