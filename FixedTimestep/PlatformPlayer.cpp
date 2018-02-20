#include "PlatformPlayer.h"

#include "Input.h"


PlatformPlayer::PlatformPlayer(int startingHP, glm::vec2 pos, float jumpHeight):Sphere(pos,glm::vec2(0,0), 10, 5, glm::vec4(0,1,1,1)), m_health(startingHP), m_jumpHeight(jumpHeight) 
{
}

void PlatformPlayer::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	Sphere::fixedUpdate(gravity, timeStep);//Physics

	//Controls
	aie::Input* input = aie::Input::getInstance();

	if ((input->wasKeyPressed(aie::INPUT_KEY_W) || input->wasKeyPressed(aie::INPUT_KEY_SPACE))) {
		//jump
		if(m_velocity.y < 0)
			applyForce(glm::vec2(0, m_jumpHeight), glm::vec2(0, 0));
	}

}

PlatformPlayer::~PlatformPlayer()
{
}
