#include "PhysicsScene.h"



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
	std::vector<PhysicsObject *>::iterator pos = m_actors.begin();
	while (pos != m_actors.end() || *pos != actor) {
		pos++;
	}
	if (pos != m_actors.end()) {
		m_actors.erase(pos);
	}
	//m_actors.erase(POINTER);
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
		checkForCollision();
	
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
	for (auto pActor : m_actors) {
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}

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