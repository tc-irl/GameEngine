#pragma once
#include "Camera.h"
#include "Window.h"
#include "Shader.h"
#include "FPSCamera.h"
class Game
{
public:
	virtual void run() = 0;

public:
	Camera *camera;
	//FPSCamera *camera;
	Window *window;
	Shader *shader;
	TwBar *bar;
	std::string vs, ps;
};

