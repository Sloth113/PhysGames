#include "PhysicsScene.h"
#include <algorithm>



PhysicsScene::PhysicsScene() :m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}


PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	m_actors.insert(m_actors.end(), actor);
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
	
	for (std::vector<PhysicsObject*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
		if (actor == (*it)) {
			PhysicsObject * delThis = (*it);
			it = m_actors.erase(it);
			//CHECK IF SPRING 
			
			delete delThis;
		}

		if (it == m_actors.end()) {
			break;
		}
	}
}

void PhysicsScene::update(float dt) {
	// update physics at a fixed time step
	static std::list<PhysicsObject*> dirty;

	//update at fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

		//Collisions
		// check for collisions (ideally you'd want to have some sort of 
		// scene management in place)
		/*
		for (auto pActor : m_actors) {
			for (auto pOther : m_actors) {
				if (pActor == pOther)
					continue;
				if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&	std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
					continue;

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
				
				if (pRigid->checkCollision(pOther) == true) {
					Rigidbody* other = dynamic_cast<Rigidbody*>(pOther);
					glm::vec2 force = other->getVelocity() * other->getMass();
					float massDif = pRigid->getMass() / other->getMass();
					pRigid->applyForceToActor(dynamic_cast<Rigidbody*>(pOther),	pRigid->getVelocity() * pRigid->getMass() / massDif);	
					dirty.push_back(pRigid);
					other->applyForceToActor(pRigid, force / massDif);
					dirty.push_back(pOther);
				}
			}
		}
		
		dirty.clear();
		*/
		//Collision check function
		checkForCollision();
		//Then clean up dead
		std::vector<PhysicsObject *> delList = m_actors;
		std::remove_if(m_actors.begin(), m_actors.end(), [](PhysicsObject * p) {return p->isDead(); });
		std::remove_if(delList.begin(), delList.end(), [](PhysicsObject * p) {return !p->isDead(); });
	}

}


void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors) {
		pActor->makeGizmo();
	}
}

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

/* Replaced with virtual functions within physics obect*/
/*
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, 
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere,

};
*/

void PhysicsScene::checkForCollision() {
	int actorCount = m_actors.size();

	//Check for all collisions expect this one
	for (int outer = 0; outer < actorCount - 1; outer++) {
		for (int inner = outer + 1; inner < actorCount; inner++) {
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];

			if (object1->getShapeID() < 0 || object2->getShapeID() < 0)
				continue;
			object1->Collide(object2);
			//int shapeId1 = object1->getShapeID();
			//int shapeId2 = object2->getShapeID();
			
			//function pointers
			/*
			int functionIdx = (shapeId1 * ShapeType::SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr) {
				collisionFunctionPtr(object1, object2);
			}
			*/
		}
	}
}