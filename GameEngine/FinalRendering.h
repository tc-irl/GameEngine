#pragma once
#include "Game.h"
#include "MeshLoader.h"
#include "Texture.h"
#include "Lighting.h"
#include "CubeMapTexture.h"
#include "Skybox.h"

class FinalRendering : public Game
{
public:

	FinalRendering(void);
	~FinalRendering(void) {};

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

	std::map <Shader::ShaderType, GLuint> possibleShaders;
	std::map <MeshLoader::RefractionIndex, float> refractions;

	Shader *skyBoxShader,*texturedShader,*normalMapShader, *reliefMapShader, *pomShader, *pomShadowShader;

	Lighting *cubeLight;

	TwBar *bar;

	MeshLoader *plane, *cube;
	Skybox *skybox;

	GLuint gSampler;

	double time, dt;// Current time and elapsed time

	bool pauseScene;
};

