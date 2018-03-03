#include "PhysicsScene.h"
#include <algorithm>


//Default timestep and gravity set here 
PhysicsScene::PhysicsScene() :m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}
//Delete list of actors MEMORY
PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}
//Add actor to the scene
void PhysicsScene::addActor(PhysicsObject * actor)
{
	m_actors.insert(m_actors.end(), actor);
}
//Remove actor from scene (will be set to destroy and cleared at end of next update.) Future me should change to destroy here.
void PhysicsScene::removeActor(PhysicsObject * actor)
{
	actor->setDestroy();
}
//Fixed update loop
void PhysicsScene::update(float dt) {

	//update at fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;
	//Once enough time has passed do an update on all actors 
	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

		//Collision check function
		checkForCollision();
		//


		//Deletion cleanup
		//Get all springs attached to this actor and destroy them
		for (std::vector<PhysicsObject*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
			if (Spring * spring = dynamic_cast<Spring*>(*it)) {
				if ((spring->getBody1())->isDead() || (spring->getBody2())->isDead()) {
					spring->setDestroy();
					//will be destroyed at end of update 
				}
			}
		}
		//Then clean up dead // Go through list if marked for dead remove from list and delete data
		for (std::vector<PhysicsObject*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
			if ((*it)->isDead()) {
				PhysicsObject * del = *it;
				it = m_actors.erase(it);
				delete del;
			}
			if (it == m_actors.end()) {
				break;
			}
		}
	}

}
//Draw each actor
void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors) {
		pActor->makeGizmo();
	}
}
//Debug info, this calculates kinetic, was not used for when rotational and potential energy was introduced
void PhysicsScene::debugScene()
{
	int count = 0;
	float sysEnergy = 0;
	for (auto pActor : m_actors) {
		Rigidbody * cast = dynamic_cast<Rigidbody * >(pActor);
		if (cast != nullptr) {
			sysEnergy += (glm::dot(cast->getVelocity(), cast->getVelocity())) * cast->getMass() / 2.0f;//Linear
			//sysEnergy += (cast->getMoment() * cast->getAngularVel()//Rotational
		}
		cast = nullptr;
		/*
		std::cout << count << " : ";
		pActor->debug();
		count++;
		std::cout << std::endl;
		*/
	}
	std::cout << sysEnergy << std::endl;

}
//Collision check each object is check with each other object
void PhysicsScene::checkForCollision() {
	int actorCount = m_actors.size();

	//Check for all collisions expect this one
	for (int outer = 0; outer < actorCount - 1; outer++) {
		for (int inner = outer + 1; inner < actorCount; inner++) {
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			//If the actor is a join dont check collision, could update to include dead
			if (object1->getShapeID() < 0 || object2->getShapeID() < 0)
				continue;
			object1->Collide(object2);
		}
	}
}