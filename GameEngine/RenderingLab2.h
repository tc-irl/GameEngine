#pragma once
#include "Game.h"
#include "MeshLoader.h"
#include "Texture.h"
#include "Lighting.h"
#include "CubeMapTexture.h"

class RenderingLab2 : public Game
{
public:

	RenderingLab2(void);
	~RenderingLab2(void) {};

	void run() override;
	void init(char** argv);
	void initTweakBar();
	void initShaders();
	void initModels();
	void initLights();
	void initTextures();
	void update();
	void UpdateLighting(GLuint shaderID, Lighting *light);
	void initRefractions();
	//std::map <std::string, GLuint> possibleShaders;
	std::map <Shader::ShaderType, GLuint> possibleShaders;
	std::map <MeshLoader::RefractionIndex, float> refractions;

	Shader *skyBoxShader, *reflectionShader, *refractionShader, *combinedShader;

	Lighting *monkeyLight, *teapotLight, *headLight;

	TwBar *bar;

	MeshLoader *plane, *cube, *teapot, *monkey, *head;
	MeshLoader *skybox;

	GLuint gSampler;

	double time, dt;// Current time and elapsed time

	bool pauseScene;

};

