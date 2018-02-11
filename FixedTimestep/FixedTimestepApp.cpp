#include "FixedTimestepApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

FixedTimestepApp::FixedTimestepApp() {

}

FixedTimestepApp::~FixedTimestepApp() {

}

bool FixedTimestepApp::startup() {
	
	// increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);


	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	//m_physicsScene->setGravity(glm::vec2(0, -9.8f));
	m_physicsScene->setTimeStep(0.01f);

	//Balls towards planes
	Sphere* ball1 = new Sphere(glm::vec2(30, 50), glm::vec2(0, 0), 8.0f, 4, glm::vec4(1, 0, 1, 1));
	//ball1->setStatic(true);
	Sphere* ball2 = new Sphere(glm::vec2(160, 100), glm::vec2(0, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	//Ball on ball
	Sphere* ball3 = new Sphere(glm::vec2(122, 70), glm::vec2(0, 0), 18.0f, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball4 = new Sphere(glm::vec2(120, 50), glm::vec2(0, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	//
	Sphere* ball5 = new Sphere(glm::vec2(90, 50), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball6 = new Sphere(glm::vec2(182, 50), glm::vec2(0, 0), 8.0f, 4, glm::vec4(1, 0, 1, 1));

	m_physicsScene->setGravity(glm::vec2(0, 0));  // turn off gravity


	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(ball3);
	m_physicsScene->addActor(ball4);
	m_physicsScene->addActor(ball5);
	//m_physicsScene->addActor(ball6);

	//ball1->applyForceToActor(ball2, glm::vec2(20, 0));
	ball1->applyForce(glm::vec2(100, 0));
	ball2->applyForce(glm::vec2(0, -100));
	ball3->applyForce(glm::vec2(0, -100));
	ball4->applyForce(glm::vec2(0, 10));
	ball5->applyForce(glm::vec2(-230, 120));
	
	//ball4->setStatic(true);
	//ball3->applyForce(glm::vec2(-40, 0));
	
	//Bounds
	float aspRatio = 16 / 9.f;
	Plane* bottom = new Plane(glm::vec2(0, 1), 2);
	Plane* top = new Plane(glm::vec2(0, 1), (200.0f / aspRatio) - 2.0f);
	Plane* left = new Plane(glm::vec2(1, 0), 2);
	Plane* right = new Plane(glm::vec2(1, 0),198);
	
	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(top);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);


	Plane* plane1 = new Plane(glm::vec2(1, 0), 100);
	Plane* plane2 = new Plane(glm::vec2(0, 1), 20);
	Plane* plane3 = new Plane(glm::vec2(1, -1), 10);
	
	m_physicsScene->addActor(plane1);
	m_physicsScene->addActor(plane2);
	m_physicsScene->addActor(plane3);

	Box* box1 = new Box(glm::vec2(20, 20), glm::vec2(0, 0), 45, 1, glm::vec2(10, 10), glm::vec4(0, 1, 1, 1));

	//m_physicsScene->addActor(box1);

	Line * line1 = new Line(glm::vec2(60, 60), glm::vec2(0, 0), 0.0f, 1.0f, 10.0f, glm::vec4(0, 1, 0, 1));
	//m_physicsScene->addActor(line1);


	return true;
}

void FixedTimestepApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void FixedTimestepApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();
	
	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void FixedTimestepApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(0, 200, 0 / aspectRatio, 200 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}