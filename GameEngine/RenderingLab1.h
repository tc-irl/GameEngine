#pragma once
#include "Game.h"
#include "Mesh.h"
#include "Texture.h"

class RenderingLab1 : public Game
{
public:
	RenderingLab1(void);
	~RenderingLab1(void) {};

	void run() override;
	void init(char** argv);
	void initTweakBar();
	void initShaders() override;
	void initModels();
	void initTextures();
	void update();

	Shader *shader1, *shader2;
	std::string ps2,vs2;
	TwBar *bar;
	Mesh *cube, *plane, *sphere, *torus, *pyramid, *monkey;

	GLuint gSampler;

	Texture *cubeTexture;

	double time, dt;// Current time and elapsed time

};

