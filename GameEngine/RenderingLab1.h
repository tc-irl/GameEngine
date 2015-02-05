#pragma once
#include "Game.h"
#include "MeshLoader.h"
#include "Texture.h"
#include "Lighting.h"

class RenderingLab1 : public Game
{
public:

	RenderingLab1(void);
	~RenderingLab1(void) {};

	void run() override;
	void init(char** argv);
	void initTweakBar();
	void initShaders();
	void initModels();
	void initLights();
	void initTextures();
	void update();
	void UpdateLighting(GLuint shaderID, Lighting *light);

	//std::map <std::string, GLuint> possibleShaders;
	std::map <Shader::ShaderType, GLuint> possibleShaders;

	Shader *basicShader, *textureShader, *toonShader, *toonTextured, *phongShader, 
			*diffuseShader, *phongTextured, *orenTextured, *orenNayar;

	Lighting *monkeyLight, *teapotLight, *headLight;

	std::string ps2,vs2;
	TwBar *bar;
	//SimpleMesh *cube, *plane, *sphere, *torus, *monkey, *goldenMonkey, *teapot;
	MeshLoader *plane, *cube, *teapot, *monkey, *head;

	GLuint gSampler;

	Texture *cubeTexture;

	double time, dt;// Current time and elapsed time

	bool pauseScene;

};

