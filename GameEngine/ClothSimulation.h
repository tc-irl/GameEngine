#pragma once

#include "Game.h"
#include "MeshLoader.h"
#include "Texture.h"
#include "Cloth.h"
#include "Lighting.h"
#include "RigidBody.h"
#include <string>

using namespace std;

#define MAX 6

class ClothSimulation : public Game
{
public:

	ClothSimulation(void);
	~ClothSimulation(void);

	void run() override;
	void init(char **argv);
	void initShaders();
	void initTweakBar();
	void initModels();
	void initTextures();
	void initLights();
	void update();
	void UpdateLighting(GLuint shaderID, Lighting *light);
	float RandomNumber(float Min, float Max);

	Cloth *cloth;
	MeshLoader *plane, *ball;

	MeshLoader *balls[MAX];
	Shader *basicShader, *textureShader;
	Line *line;
	std::map <Shader::ShaderType, GLuint> possibleShaders;

	bool pauseScene;
	double time, dt;// Current time and elapsed time
	glm::vec3 forceDirection;
	glm::vec3 windDirection;

	Lighting *light;
};

