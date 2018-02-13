#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Line.h"
#include <stdlib.h>
#include <time.h>

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


protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene*		m_physicsScene;
	Sphere* bigBall;
	Box * testB;
};