#pragma once
#include "Sphere.h"

class PlatformPlayer :	public Sphere
{
public:
	PlatformPlayer(int startingHP, glm::vec2 pos, float jumpHeight);
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	~PlatformPlayer();
protected:
	int m_health;
	int m_score;
	float m_jumpHeight;
	float m_safeZone;
	bool m_jumping;
};

