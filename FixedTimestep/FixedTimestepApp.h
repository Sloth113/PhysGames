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
class FixedTimestepApp : public aie::Application {
public:

	FixedTimestepApp();
	virtual ~FixedTimestepApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	//Set up scenes
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

	PhysicsScene*		m_physicsScene;
	Sphere* bigBall;
	Box * testB;
	float ballSize;
	Rigidbody * m_hitThis;
	Rigidbody * m_moveThis;
	bool m_oldKinState;
	glm::vec2 m_clickPos;

};