#pragma once

#include "Application.h"
#include "Renderer2D.h"

class Tutorial1App : public aie::Application {
public:

	Tutorial1App();
	virtual ~Tutorial1App();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	float				m_newVar;
	
};