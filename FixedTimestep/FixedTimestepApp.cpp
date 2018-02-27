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

	std::srand((int)std::time(NULL));
	ballSize = 0;
	m_clickPos = glm::vec2(0, 0);
	m_hitThis = nullptr;
	m_moveThis = nullptr;
	//SphereWallsCollisions();
	//TestingScene(); //circle intersection 
	//CradleTest();
	//Overload();
	//AllTheShapes();
	//SpringTests();
	Platformer();
	//BallandCorner();

	/*
	m_physicsScene = new PhysicsScene();
	//m_physicsScene->setGravity(glm::vec2(0, -50.0f));
	m_physicsScene->setTimeStep(0.01f);

	bigBall = new Sphere(glm::vec2(10, 50), glm::vec2(0, 0), 10, 5.0f, glm::vec4(0, 1, 1, 1));
	m_physicsScene->addActor(bigBall);

	Box * box1 = new Box(glm::vec2(50, 30), glm::vec2(0, 0), 45, 4, glm::vec2(5, 5), glm::vec4(0, 1, 1, 1));
	Box * box2 = new Box(glm::vec2(52, 28), glm::vec2(0, 0), 45, 4, glm::vec2(5, 5), glm::vec4(1, 0, 1, 1));

	m_physicsScene->addActor(box1);
	m_physicsScene->addActor(box2);
	*/
	/*float aspRatio = 16 / 9.f;
	Plane* bottom = new Plane(glm::vec2(0, 1), 2);
	Plane* top = new Plane(glm::vec2(0, 1), (200.0f / aspRatio) - 2.0f);
	Plane* left = new Plane(glm::vec2(1, 0), 2);
	Plane* right = new Plane(glm::vec2(1, 0), 198);

	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(top);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);

	Box * box = new Box(glm::vec2(rand() % 100 + 5, (rand() & 100) + 5), glm::vec2(rand() % 10 + 1, rand() % 10 + 1), 0, 4, glm::vec2(4, 4), glm::vec4(1, 0, 0, 1));

	m_physicsScene->addActor(box);
	*/

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
	//Spawn balls
	/*
	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
		glm::vec2 click = glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * 198, (float)input->getMouseY() / (float)getWindowHeight() * 115);

		//m_physicsScene->addActor(new Sphere(click, glm::vec2(0, 0), 1.0f, 3, glm::vec4(0, 1, 0, 1)));
		Box * newBox = new Box(click, glm::vec2(0, 0), 0, 1, glm::vec2(2, 2), glm::vec4(0, 0, 1, 1));
		newBox->setKinematic(true);
		
		m_physicsScene->addActor(newBox);
	}
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT )&& bigBall != nullptr) {
		glm::vec2 click = glm::vec2((float)input->getMouseX() / (float)getWindowWidth() * 198, (float)input->getMouseY() / (float)getWindowHeight() * 115);
		
		bigBall->applyForce(glm::vec2( glm::normalize(click - bigBall->getPosition())) * 100.0f, glm::vec2(0,0));
	}*/
	//Mouse
	float aspRatio = 16 / 9.f;
	glm::vec2 click = glm::vec2((float)input->getMouseX() / getWindowWidth() * 1000, (float)input->getMouseY() / getWindowHeight() * 1000 / aspRatio );
	aie::Gizmos::add2DCircle(click, 1, 16, glm::vec4(0, 1, 1, 1));

	//Spawn ball on left click 
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT)) {
		ballSize += deltaTime * 50;
		aie::Gizmos::add2DCircle(click, ballSize, 32, glm::vec4(1, 0, 1, 1));
	}
	if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT) && ballSize > 0) {
		Sphere * ball = new Sphere(click, glm::vec2(0, 0), ballSize , ballSize, glm::vec4(0, 0, 1, 1));
		m_physicsScene->addActor(ball);
		ballSize = 0;
	}
	//Right click to give objects force
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
		//if(m_clickPos == glm::vec2(0,0))
			//m_clickPos = click;
		if (m_hitThis == nullptr) {
			for (PhysicsObject * p : (m_physicsScene->getActors())) {
				if (Rigidbody * r = dynamic_cast<Rigidbody*>(p)) {
					Sphere * tmp = new Sphere(click, glm::vec2(0, 0), 1, 1, glm::vec4(0, 1, 1, 1));
					if (r->checkCollision(tmp)) {
						m_hitThis = r;
						m_clickPos = click;
					}
					delete tmp;
				}
			}
		}
		else {
			aie::Gizmos::add2DLine(m_clickPos, click, glm::vec4(0, 1, 1, 1));
		}
	}
	if (input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_RIGHT) && m_clickPos != glm::vec2(0,0)) {
		//m_hitThis->applyForce((click - m_clickPos) * 5.0f, click - m_hitThis->getPosition());
		m_hitThis->applyForce((click - m_clickPos ) * 10.0f, m_clickPos - m_hitThis->getPosition());
		m_clickPos = glm::vec2(0, 0);
		m_hitThis = nullptr;
	}
	//Click and drag middle mouse to move items 
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_MIDDLE)) {
		if (m_moveThis == nullptr) {
			for (PhysicsObject * p : (m_physicsScene->getActors())) {
				if (Rigidbody * r = dynamic_cast<Rigidbody*>(p)) {
					Sphere * tmp = new Sphere(click, glm::vec2(0, 0), 1, 1, glm::vec4(0, 1, 1, 1));
					if (r->checkCollision(tmp)) {
						m_moveThis = r;
					}
					delete tmp;
				}
			}
		}
		else {
			m_moveThis->setPosition(click);
		}
	}
	if (input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_MIDDLE) && m_moveThis != nullptr) {
		m_moveThis = nullptr;
	}

	if (input->wasKeyPressed(aie::INPUT_KEY_DELETE) || input->wasKeyPressed(aie::INPUT_KEY_SPACE)) {
		for (PhysicsObject * p : (m_physicsScene->getActors())) {
			if (Rigidbody * r = dynamic_cast<Rigidbody*>(p)) {
				Sphere * tmp = new Sphere(click, glm::vec2(0, 0), 1, 1, glm::vec4(0, 1, 1, 1));
				if (r->checkCollision(tmp)) {
					m_physicsScene->removeActor(p);
					//p->setDestroy();
				}
				delete tmp;
			}
		}
	}


	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	//m_physicsScene->debugScene();
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
	aie::Gizmos::draw2D(glm::ortho<float>(0, 1000, 0, 1000 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void FixedTimestepApp::AllTheShapes()
{
	m_physicsScene = new PhysicsScene();
	bigBall = new Sphere(glm::vec2(10, 50), glm::vec2(0, 0), 1, 5.0f, glm::vec4(0, 1, 1, 1));
	m_physicsScene->addActor(bigBall);
	
	//m_physicsScene->setGravity(glm::vec2(0, -10));

	float aspRatio = 16 / 9.f;
	Plane* bottom = new Plane(glm::vec2(0, 1), 2);
	Plane* top = new Plane(glm::vec2(0, 1), (200.0f / aspRatio) - 2.0f);
	Plane* left = new Plane(glm::vec2(1, 0), 2);
	Plane* right = new Plane(glm::vec2(1, 0), 198);

	testB = new Box(glm::vec2(56, 60), glm::vec2(0, 0), 0.0f, 10, glm::vec2(5, 8), glm::vec4(1, 0, 0, 1));
	testB->applyForce(glm::vec2(0, -10), testB->getPosition());
	m_physicsScene->addActor(testB);

	Box* testB2 = new Box(glm::vec2(76, 80), glm::vec2(0, 0), 0.0f, 10, glm::vec2(1, 10), glm::vec4(1, 0, 0, 1));
	testB2->applyForce(glm::vec2(0, -10), testB->getPosition());
	m_physicsScene->addActor(testB2);


	Box* testB3 = new Box(glm::vec2(106, 80), glm::vec2(0, 0), 0.0f, 10, glm::vec2(3, 3), glm::vec4(1, 0, 0, 1));
	testB3->applyForce(glm::vec2(0, -30), testB->getPosition());
	m_physicsScene->addActor(testB3);

	Box* testB4 = new Box(glm::vec2(72, 80), glm::vec2(0, 0), 0.0f, 10, glm::vec2(1, 10), glm::vec4(1, 0, 0, 1));
	
	m_physicsScene->addActor(testB4);

	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(top);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);
}

void FixedTimestepApp::SpringTests()
{
	m_physicsScene = new PhysicsScene();
	//m_physicsScene->setGravity(glm::vec2(0, -50.0f));
	m_physicsScene->setTimeStep(0.01f);

	bigBall = new Sphere(glm::vec2(10, 50), glm::vec2(0, 0), 10, 5.0f, glm::vec4(0, 1, 1, 1));
	m_physicsScene->addActor(bigBall);

	Sphere * oldBall = new Sphere(glm::vec2(20, 55), glm::vec2(0, 0), 5, 2.0f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(oldBall);

	Spring * attach = new Spring(bigBall, oldBall, 1, 10.0f, 0, glm::vec2(5, 0), glm::vec2(-2,0));
	m_physicsScene->addActor(attach);

	for (int i = 0; i < 20; i++) {
		Sphere * nextBall = new Sphere(glm::vec2(20 + i * 5, 60), glm::vec2(0, 0), 5, 2.0f, glm::vec4(1, 0, 0, 1));
		nextBall->setElasticity(0.5f);
		m_physicsScene->addActor(nextBall);
		m_physicsScene->addActor(new Spring(oldBall, nextBall, 1, 1000, 0, glm::vec2(2,0), glm::vec2(-2,0)));
		oldBall = nextBall;
	}
	oldBall->setKinematic(true);
	//Bounds
	float aspRatio = 16 / 9.f;
	Plane* bottom = new Plane(glm::vec2(0, 1), 2);
	Plane* top = new Plane(glm::vec2(0, 1), (200.0f / aspRatio) - 2.0f);
	Plane* left = new Plane(glm::vec2(1, 0), 2);
	Plane* right = new Plane(glm::vec2(1, 0), 198);

	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(top);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);


}

void FixedTimestepApp::Platformer()
{
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -100));
	m_physicsScene->setTimeStep(0.01f);
	//Player
	PlatformPlayer * player = new PlatformPlayer(10, glm::vec2(10, 100), 500);
	player->applyForce(glm::vec2(10, 0), glm::vec2(0, 0));
	player->setLinearDrag(0);//doesnt slow down?
	//m_physicsScene->addActor(player);

	//Bounds NOW IN SOLID FORM
	float aspRatio = 16 / 9.f;
	Box* bottom =	new Box(glm::vec2(500, 0), glm::vec2(0, 0), 0, 500, glm::vec2(1000, 10), glm::vec4(0.5f, 0.5f, 0.5f, 1));
	Box* top =		new Box(glm::vec2(500, 1000 / aspRatio), glm::vec2(0, 0), 0, 500, glm::vec2(1000, 10), glm::vec4(0.5f, 0.5f, 0.5f, 1));
	Box* left =		new Box(glm::vec2(0, 500 / aspRatio), glm::vec2(0, 0), 0, 500, glm::vec2(10, 600), glm::vec4(0.5f, 0.5f, 0.5f, 1));
	Box* right =	new Box(glm::vec2(1000, 500 / aspRatio), glm::vec2(0, 0), 0, 500, glm::vec2(10, 600), glm::vec4(0.5f, 0.5f, 0.5f, 1));


	bottom->setKinematic(true);
	top->setKinematic(true);
	left->setKinematic(true);
	right->setKinematic(true);


	bottom->setElasticity(0.9f);
	top->setElasticity(0.9f);
	left->setElasticity(0.9f);
	right->setElasticity(0.9f);

	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(top);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);

	//Level bits
	Box * platform1 = new Box(glm::vec2(10, 50), glm::vec2(0, 0), -20, 100, glm::vec2(10, 2), glm::vec4(0, 1, 0, 1));
	platform1->setElasticity(1);
	platform1->setKinematic(true);
	m_physicsScene->addActor(platform1);

	Sphere * anchor1 = new Sphere(glm::vec2(50, 80), glm::vec2(0, 0), 100, 2, glm::vec4(0, 0, 1, 1));
	anchor1->setKinematic(true);
	Sphere * anchor2 = new Sphere(glm::vec2(155, 80), glm::vec2(0, 0), 100, 2, glm::vec4(0, 0, 1, 1));
	anchor2->setKinematic(true);
	Box * movingPlat1 = new Box(glm::vec2(65, 80), glm::vec2(0, 0), 0, 10, glm::vec2(10, 1), glm::vec4(0, 1, 0, 1));
	Box * movingPlat2 = new Box(glm::vec2(90, 80), glm::vec2(0, 0), 0, 10, glm::vec2(10, 1), glm::vec4(0, 1, 0, 1));
	Box * movingPlat3 = new Box(glm::vec2(115, 80), glm::vec2(0, 0), 0, 10, glm::vec2(10, 1), glm::vec4(0, 1, 0, 1));
	Box * movingPlat4 = new Box(glm::vec2(140, 80), glm::vec2(0, 0), 0, 10, glm::vec2(10, 1), glm::vec4(0, 1, 0, 1));
	movingPlat1->setLinearDrag(1.0f);
	movingPlat2->setLinearDrag(1.0f);
	movingPlat3->setLinearDrag(1.0f);
	movingPlat4->setLinearDrag(1.0f);
	movingPlat1->setAngularDrag(2.0f);
	movingPlat2->setAngularDrag(2.0f);
	movingPlat3->setAngularDrag(2.0f);
	movingPlat4->setAngularDrag(2.0f);
	Spring * attach1 = new Spring(anchor1, movingPlat1, 1, 100, 0.1f, glm::vec2(0,0), glm::vec2(-10,0));
	Spring * attach2 = new Spring(movingPlat1, movingPlat2, 1, 100, 0.1f, glm::vec2(10, 0), glm::vec2(-10, 0));
	Spring * attach3 = new Spring(movingPlat2, movingPlat3, 1, 100, 0.1f, glm::vec2(10, 0), glm::vec2(-10, 0));
	Spring * attach4 = new Spring(movingPlat3, movingPlat4, 1, 100, 0.1f, glm::vec2(10, 0), glm::vec2(-10, 0));
	Spring * attach5 = new Spring(movingPlat4, anchor2, 1, 100, 0.1f, glm::vec2(10, 0), glm::vec2(0, 0));
	m_physicsScene->addActor(anchor1);	
	m_physicsScene->addActor(anchor2);
	m_physicsScene->addActor(movingPlat1);
	m_physicsScene->addActor(movingPlat2);
	m_physicsScene->addActor(movingPlat3);
	m_physicsScene->addActor(movingPlat4);
	m_physicsScene->addActor(attach1);
	m_physicsScene->addActor(attach2);
	m_physicsScene->addActor(attach3);
	m_physicsScene->addActor(attach4);
	m_physicsScene->addActor(attach5);

	Sphere * ball1 = new Sphere(glm::vec2(500, 100), glm::vec2(0, 10), 1, 30, glm::vec4(0, 1, 1, 1));
	m_physicsScene->addActor(ball1);

}

void FixedTimestepApp::BallandCorner()
{
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);
	//m_physicsScene->setGravity(glm::vec2(0,-100.0f));
	//Bounds
	float aspRatio = 16 / 9.f;
	Plane* bottom = new Plane(glm::vec2(0, 1), 2);
	Plane* top = new Plane(glm::vec2(0, 1), 1000 / aspRatio - 2.0f);
	Plane* left = new Plane(glm::vec2(1, 0), 2);
	Plane* right = new Plane(glm::vec2(1, 0), 1000 - 2.0f);

	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(top);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);

	Box * box1 = new Box(glm::vec2(500, 200), glm::vec2(0, 0), 0, 10, glm::vec2(40, 40), glm::vec4(1, 1, 1, 1));
	Sphere * ball1 = new Sphere(glm::vec2(501, 199), glm::vec2(0, 0), 10, 10, glm::vec4(1, 1, 0, 1));
	//box1->setKinematic(true);
	box1->setAngularDrag(1.0f);
	box1->setLinearDrag(1.0f);

	Box * box2 = new Box(glm::vec2(700, 300), glm::vec2(0, 0), 0, 10, glm::vec2(50, 10), glm::vec4(1, 1, 1, 1));
	Sphere * ball2 = new Sphere(glm::vec2(704, 299), glm::vec2(0, 0), 10, 5, glm::vec4(1, 1, 0, 1));
	//box1->setKinematic(true);
	
	m_physicsScene->addActor(box1);
	m_physicsScene->addActor(ball1);

	m_physicsScene->addActor(box2);
	m_physicsScene->addActor(ball2);

}

void FixedTimestepApp::SphereWallsCollisions()
{
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -9.8f));
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
	ball1->applyForce(glm::vec2(100, 0), glm::vec2(0));
	ball2->applyForce(glm::vec2(0, -100), glm::vec2(0));
	ball3->applyForce(glm::vec2(0, -100), glm::vec2(0));
	ball4->applyForce(glm::vec2(0, 10), glm::vec2(0));
	ball5->applyForce(glm::vec2(-230, 120), glm::vec2(0));

	//ball4->setStatic(true);
	//ball3->applyForce(glm::vec2(-40, 0));

	//Bounds
	float aspRatio = 16 / 9.f;
	Plane* bottom = new Plane(glm::vec2(0, 1), 2);
	Plane* top = new Plane(glm::vec2(0, 1), (200.0f / aspRatio) - 2.0f);
	Plane* left = new Plane(glm::vec2(1, 0), 2);
	Plane* right = new Plane(glm::vec2(1, 0), 198);

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
}

void FixedTimestepApp::TestingScene()
{
	//m_physicsScene->setGravity(glm::vec2(0, -9.8f));
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);
	//Bounds
	float aspRatio = 16 / 9.f;
	Plane* bottom = new Plane(glm::vec2(0, 1), 2);
	Plane* top = new Plane(glm::vec2(0, 1), (200.0f / aspRatio) - 2.0f);
	Plane* left = new Plane(glm::vec2(1, 0), 2);
	Plane* right = new Plane(glm::vec2(1, 0), 198);

	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(top);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);

	//Ball on ball
	Sphere* ball1 = new Sphere(glm::vec2(50, 50), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(56, 50), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball3 = new Sphere(glm::vec2(100, 50), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 1, 1, 1));
	ball3->setKinematic(true);
	ball1->applyForce(glm::vec2(150, 1), glm::vec2(0));
	ball2->applyForce(glm::vec2(100, 4), glm::vec2(0));

	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);	
	m_physicsScene->addActor(ball3);
	
}

void FixedTimestepApp::CradleTest()
{
	//m_physicsScene->setGravity(glm::vec2(0, -9.8f));
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);
	//Bounds
	float aspRatio = 16 / 9.f;
	Plane* bottom = new Plane(glm::vec2(0, 1), 2);
	Plane* top = new Plane(glm::vec2(0, 1), (200.0f / aspRatio) - 2.0f);
	Plane* left = new Plane(glm::vec2(1, 0), 2);
	Plane* right = new Plane(glm::vec2(1, 0), 198);

	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(top);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);

	//Ball on ball
	Sphere* ball1 = new Sphere(glm::vec2(50, 50), glm::vec2(0, 0), 2.0f, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(60, 50), glm::vec2(0, 0), 1.0f, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball3 = new Sphere(glm::vec2(70, 50), glm::vec2(0, 0), 1.0f, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball4 = new Sphere(glm::vec2(80, 50), glm::vec2(0, 0), 1.0f, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball5 = new Sphere(glm::vec2(90, 50), glm::vec2(0, 0), 1.0f, 4, glm::vec4(1, 0, 0, 1));

	ball1->applyForce(glm::vec2(200, 0), glm::vec2(0));

	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(ball3);
	m_physicsScene->addActor(ball4);
	m_physicsScene->addActor(ball5);
}

void FixedTimestepApp::Overload()
{
	//m_physicsScene->setGravity(glm::vec2(0, -9.8f));
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);
	//Bounds
	float aspRatio = 16 / 9.f;
	Plane* bottom = new Plane(glm::vec2(0, 1), 2);
	Plane* top = new Plane(glm::vec2(0, 1), (200.0f / aspRatio) - 2.0f);
	Plane* left = new Plane(glm::vec2(1, 0), 2);
	Plane* right = new Plane(glm::vec2(1, 0), 198);

	m_physicsScene->addActor(bottom);
	m_physicsScene->addActor(top);
	m_physicsScene->addActor(left);
	m_physicsScene->addActor(right);

	//


	Sphere * balls[100];
	for (int i = 0; i < 100; i++) {
		
		if (rand() % 10 == 0) {
			balls[i] = new Sphere(glm::vec2(rand() % 190 + 5, rand() % 100 + 5), glm::vec2(0, 0), 10.0f, 3, glm::vec4(0.5f, 0.5f, 0.5f, 1));
			m_physicsScene->addActor(balls[i]);
			balls[i]->setKinematic(true); //WORK OUT STATIC COLLISINOS 
		}
		else {
			balls[i] = new Sphere(glm::vec2(rand() % 190 + 5, rand() % 100 + 5), glm::vec2(0, 0), 10.0f, 3, glm::vec4(1, 1, 1, 1));
			m_physicsScene->addActor(balls[i]);
		}


	}
	
}


