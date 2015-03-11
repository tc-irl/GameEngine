#pragma once
#include "Game.h"
#include "MeshLoader.h"
#include "Texture.h"
#include "Lighting.h"
#include "CubeMapTexture.h"

class RenderingLab3 : public Game
{
public:

	RenderingLab3(void);
	~RenderingLab3(void) {};

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

	Shader *skyBoxShader, *reflectionShader, *refractionShader, *refractionDispShader, *combinedShader, *combinedDispShader, *texturedShader, *fresnelShader, *normalFresnel;

	Lighting *sphereLight, *teapotLight, *carLight, *cubeLight;

	TwBar *bar;

	MeshLoader *plane, *cube, *teapot, *monkey, *car;
	MeshLoader *skybox;

	GLuint gSampler;

	double time, dt;// Current time and elapsed time

	bool pauseScene;
	Shader *normalMapShader;
};

