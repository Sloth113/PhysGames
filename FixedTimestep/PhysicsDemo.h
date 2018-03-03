#pragma once

#include <Gizmos.h>
#include <glm\ext.hpp>
#include <stdlib.h>
#include <time.h>

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"
#include "PlatformPlayer.h"
#include "DeathBox.h"

//Provided base loop clss that works with bootstrap
//update and draw are called every frame
class PhysicsDemo : public aie::Application {
public:

	PhysicsDemo();
	virtual ~PhysicsDemo();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();
	//Set up scenes
	void PhysicsBuild();
	//
	void SphereWallsCollisions();
	void TestingScene();
	void CradleTest();
	void Overload();
	void AllTheShapes();
	void SpringTests();
	void Platformer();
	void BallandCorner();
	void SoftBody();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	//Physics engine 
	PhysicsScene*		m_physicsScene;
	//Variables used in dynamic elements in update loop 
	float m_ballSize;
	Rigidbody * m_hitThis;
	Rigidbody * m_moveThis;
	bool m_oldKinState;
	glm::vec2 m_clickPos;

};

