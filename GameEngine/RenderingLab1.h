#pragma once
#include "Game.h"
#include "Mesh.h"
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

	Shader *basicShader, *textureShader, *toonShader, *toonTexturedShader, *phongShader, *diffuseShader;
	Lighting *light, *phongLight, *diffuseLight;

	std::string ps2,vs2;
	TwBar *bar;
	Mesh *cube, *plane, *sphere, *torus, *pyramid, *monkey, *goldenMonkey, *teapot;

	GLuint gSampler;

	glm::vec3 toonLightDirection;
	// diffuse
	glm::vec3 directionalLightColor;
	glm::vec3 direction;
	float directionalIntensity;

	// ambient
	glm::vec3 ambientColor;
	float ambientIntensity;

	//specular
	glm::vec3 specularColor;
	float specularIntensity;
	float specularShininess;

	Texture *cubeTexture;

	double time, dt;// Current time and elapsed time

};

