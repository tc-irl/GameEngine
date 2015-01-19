#pragma once
#include "Camera.h"
#include "Window.h"
#include "Shader.h"
class Game
{
public:
	virtual void run() = 0;
	virtual void init() = 0;
	virtual void initShaders() = 0;

public:
	Camera *camera;
	Window *window;
	Shader *shader;
	std::string vs, ps;
};

