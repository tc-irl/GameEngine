#pragma once
#include "Camera.h"
#include "Window.h"
#include "Shader.h"
class Game
{
public:
	virtual void run() = 0;

public:
	Camera *camera;
	Window *window;
	Shader *shader;
	TwBar *bar;
	std::string vs, ps;
};

